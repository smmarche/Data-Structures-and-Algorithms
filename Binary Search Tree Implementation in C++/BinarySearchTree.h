#pragma once

#include <functional> // std::less
#include <iostream>
#include <queue> // std::queue
#include <utility> // std::pair

template <typename K, typename V, typename Comparator = std::less<K>>
class BinarySearchTree
{
  public:
    using key_type        = K;
    using value_type      = V;
    using key_compare     = Comparator;
    using pair            = std::pair<key_type, value_type>;
    using pointer         = pair*;
    using const_pointer   = const pair*;
    using reference       = pair&;
    using const_reference = const pair&;
    using difference_type = ptrdiff_t;
    using size_type       = size_t;

  private:
    struct BinaryNode
    {
        pair element;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode( const_reference theElement, BinaryNode *lt, BinaryNode *rt )
          : element{ theElement }, left{ lt }, right{ rt } { }
        
        BinaryNode( pair && theElement, BinaryNode *lt, BinaryNode *rt )
          : element{ std::move( theElement ) }, left{ lt }, right{ rt } { }
    };

    using node           = BinaryNode;
    using node_ptr       = node*;
    using const_node_ptr = const node*;

    node_ptr _root;
    size_type _size;
    key_compare comp;

  public:
    BinarySearchTree():_size(0), _root(nullptr){

    }
    BinarySearchTree( const BinarySearchTree & rhs ) :_root(nullptr), _size(0) {
        _root = clone(rhs._root);
        _size = rhs._size;
    }
    BinarySearchTree( BinarySearchTree && rhs ) : _root(nullptr), _size(0) {
        if(rhs._root != nullptr){
            std::swap(_root, rhs._root);
            std::swap(_size, rhs._size);
        }
    }
    ~BinarySearchTree() {
        clear();
    }

    const_reference min() const { return min( _root )->element; }
    const_reference max() const { return max( _root )->element; }
    const_reference root() const {
        //return const ref to the pair
        return _root->element;
    }
    bool contains( const key_type & x ) const { return contains( x, _root ); }
    value_type & find( const key_type & key ) { return find( key, _root )->element.second; }
    const value_type & find( const key_type & key ) const { return find( key, _root )->element.second; }
    bool empty() const {
        bool issize = true;
        if(_size != 0){
            issize = false;
        }
        else{
            issize = true;
        }
        return(issize);
    }
    size_type size() const {
        return _size;
    }

    void clear() {
        clear( _root );
        _size = 0;
    }
    void insert( const_reference x ) { insert( x, _root ); }
    void insert( pair && x ) { insert( std::move( x ), _root ); }
    void erase( const key_type & x ) { erase(x, _root); }

    BinarySearchTree & operator=( const BinarySearchTree & rhs ) {
        // if this and &rhs are the same don't deep copy otherwise
        //clear the current object and deep copy
        if (this != &rhs) {
            clear(); //clear this
            _root = clone(rhs._root);
            _size = rhs._size;
    }
    return *this;
    }
    BinarySearchTree & operator=( BinarySearchTree && rhs ) {
        if (this != &rhs) {
            clear();
            _root = rhs._root;
            _size = rhs._size;
            rhs._root = nullptr;
            rhs._size = 0;
    }
    return *this;
    }

  private:
    void insert( const_reference x, node_ptr & t ) {// x key-value pair to insert; t is pointer to
    //the current node in the tree
        if (t == nullptr){//the the current node is nullptr then construct a new node
            t = new BinaryNode{x,nullptr,nullptr};
            _size+=1;//increase the size
        } 
        else if (comp(x.first , t->element.first)){//compare key of key-value pair to key of the current node
            insert(x,t->left);//insert x in the left if key-value pair is less than current node
        }
        else if (comp(t->element.first, x.first)){//compare current node with x key-value pair
            insert(x, t->right);//if current node is less than key-value pair insert x on right
        }
        else{//the case that the current node and key-value pair have equal keys
            t->element.second = x.second;//update the value of the current node to the value of the x key-value pair
        }
    }
    void insert( pair && x, node_ptr & t ) {
        if (t == nullptr){
            t = new BinaryNode{std::move(x),nullptr,nullptr};
            _size++;
        }
        else if (comp(x.first , t->element.first)){
            insert(std::move(x),t->left);
        }
        else if (comp(t->element.first, x.first)){
            insert(std::move(x), t->right);
        } 
        else{
            t->element.second = std::move(x.second);
        }
    }

    void erase( const key_type & x, node_ptr & t ) {
        if (t == nullptr) {//if there's nothing to erase don't do anything
            return;
        }
        //if, else if statements below essentially search for the target x key to erase
        if (comp(x, t->element.first)) {//check if the x key is less than the node key
            erase(x, t->left);//the function recursively calls itself with the left child of the root node as the new root node
        } 
        else if (comp(t->element.first, x)) {//check if the node key is less than x key
            erase(x, t->right);//the function recursively calls itself with the right child of the root node as the new root
        } 

        //once t key is = to x key erasing happens
        //erasing with two children
        else if (t->left != nullptr && t->right != nullptr) {
            t->element = min(t->right)->element;//find the right most left node or the minimum element in the rightmost subtree
            //in the line above we are overwritting t with the rightmost left node
            erase(t->element.first, t->right);//rightmost left node key, and it's child
        } 
        //removes a child from the tree when it has one child or no children
        else {
            node_ptr previousNode = t;//previousNode is node to erase
            //t = (t->left != nullptr) ? t->left : t->right;
            if (t->left != nullptr) {//if the one child is left
                t = t->left;//the new head node of the subtree is the left child
            }
            else {//if the one child is right
                t = t->right;//new head node of the subtree is the right child
            }            
            delete previousNode;//deallocate memory of node to delete
            previousNode = nullptr;
            _size -= 1;
        }
    }

    const_node_ptr min( const_node_ptr t ) const {
        // recursively
        if (!t){//if true that t is null
            return nullptr;
        }
        if (t->left == nullptr){//if the left child is nullptr return t as that is the minimum
            return t;
        }
        return min(t->left);//case where the minimum is not found recursively call the function
        //until the minimum is found
    }
    const_node_ptr max( const_node_ptr t ) const {
        // nonrecursively
/*    if (!t) { |checks if t is nullptr
        return nullptr;
    }
    while (t->right != nullptr) {|runs while the right child is not null
        t = t->right;//t is assigned with the right child
    }
    return t;|returns that rightmost node*/

    //recursively
    if (!t) {
        return nullptr;//check if t is nullptr
    } else if (!t->right) {//check if t does not have a child
        return t; //if it's true t does NOT have a child it returns t since that's the max
    } else {
        return max(t->right);//otherwise it recursively calls itself and moves further down and to the right
        //to find the max value
    }
    }

    bool contains( const key_type & x, const_node_ptr t ) const {
        if (t==nullptr){//item is not found
            return false;
        } else if (comp(x, t->element.first)){//target is less than current node so look left
            return contains(x,t->left);//check the target against the left child
        } else if (comp(t->element.first, x)){//target is greater than current node so look right
            return contains(x,t->right);//check the target against the right child
        } else{//the case where x key equals t key so the target item was found
            return true;
        }        
    }
    node_ptr find( const key_type & key, node_ptr t ) {
        // cases work similarly to contains
        if (t == nullptr) {//you didn't find the key
            return nullptr;
        }
        else if (comp(key, t->element.first)) {
            return find(key, t->left);
        }
        else if (comp(t->element.first, key)) {
            return find(key, t->right);
        }
        else {//key and key of current node are equal
            return t;
        }
    }
    const_node_ptr find( const key_type & key, const_node_ptr t ) const {
        if (t == nullptr) {
            return nullptr;
        }
        else if (comp(key, t->element.first)) {
            return find(key, t->left);
        }
        else if (comp(t->element.first, key)) {
            return find(key, t->right);
        }
        else {
            return t;
        }
    }

    void clear( node_ptr & t ) {
        if (t != nullptr) {
            clear(t->left);    // clear left subtree
            clear(t->right);   // clear right subtree
            delete t;          // delete current node
            t = nullptr;       // set pointer to null
        }
    }
    
    node_ptr clone ( const_node_ptr t ) const {
        //t is root
        // base case-if t is nullptr that's the end of the tree so there's nothing to clone
        if (t==nullptr){
            return nullptr;
        }
        //recursive case: make a bunch of new nodes
        else{
            return new node{std::move (t->element), clone(t->left), clone(t->right)};
        }
    }

  public:
    template <typename KK, typename VV, typename CC>
    friend void printLevelByLevel( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out );

    template <typename KK, typename VV, typename CC>
    friend std::ostream& printNode(std::ostream& o, const typename BinarySearchTree<KK, VV, CC>::node& bn);

    template <typename KK, typename VV, typename CC>
    friend void printTree( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out );

    template <typename KK, typename VV, typename CC>
    friend void printTree(typename BinarySearchTree<KK, VV, CC>::const_node_ptr t, std::ostream & out, unsigned depth );

    template <typename KK, typename VV, typename CC>
    friend void vizTree(
        typename BinarySearchTree<KK, VV, CC>::const_node_ptr node, 
        std::ostream & out,
        typename BinarySearchTree<KK, VV, CC>::const_node_ptr prev
    );

    template <typename KK, typename VV, typename CC>
    friend void vizTree(
        const BinarySearchTree<KK, VV, CC> & bst, 
        std::ostream & out
    );
};

template <typename KK, typename VV, typename CC>
std::ostream& printNode(std::ostream & o, const typename BinarySearchTree<KK, VV, CC>::node & bn) {
    return o << '(' << bn.element.first << ", " << bn.element.second << ')';
}
//TODO
template <typename KK, typename VV, typename CC>
void printLevelByLevel( const BinarySearchTree<KK, VV, CC>& bst, std::ostream & out = std::cout ) {
    using node = typename BinarySearchTree<KK, VV, CC>::node;
    using node_ptr = typename BinarySearchTree<KK, VV, CC>::node_ptr;
    using const_node_ptr = typename BinarySearchTree<KK, VV, CC>::const_node_ptr;

    int depth = 0;
    int count = 0;
    bool thereisachild = false;
    
    if (bst._root == nullptr){ //check if the tree is empty (see if there is a root node)
        return;
    }
    std::queue<node_ptr> searchQueue; //create a queue of node_ptr objects
    searchQueue.push(bst._root); //if there is a node put it in the queue

    while (!searchQueue.empty())//this continues while the queue is not empty
    {
        node_ptr temp = searchQueue.front();// get the front element from the queue
        searchQueue.pop(); // remove the front element from the queue
        if (temp == nullptr) // the case that the front element is null
        {
            searchQueue.push(nullptr);// push two nulls to the queue (for left and right children)
            searchQueue.push(nullptr);
            out << "null "; //print null
        }
        else {// the case that the front element is not null
            searchQueue.push(temp->left);// push the left child to the queue
            searchQueue.push(temp->right);// push the right child to the queue
            if (temp->right != nullptr || temp->left != nullptr)// if at least one child is not null
            {
                thereisachild = true;// set thereisachild to true
            }
            printNode<KK, VV, CC>(out, *temp);// print the key-value pair of the front element
        }
        countt+=1;//increase count

        if (count == (1 << depth)){// if the number of printed elements is equal to 2^depth
            //if the count of nodes processed so far is equal to the total number of nodes in the current level of the tree
            count = 0;
            if (!thereisachild)//if all the children of the nodes at this level are null
            {
                break;
            }
            thereisachild = false;//thereisachild is false
            ++depth;//increase depth
        }
    }
}

template <typename KK, typename VV, typename CC>
void printTree( const BinarySearchTree<KK, VV, CC> & bst, std::ostream & out = std::cout ) { printTree<KK, VV, CC>(bst._root, out ); }

template <typename KK, typename VV, typename CC>
void printTree(typename BinarySearchTree<KK, VV, CC>::const_node_ptr t, std::ostream & out, unsigned depth = 0 ) {
    if (t != nullptr) {
        printTree<KK, VV, CC>(t->right, out, depth + 1);
        for (unsigned i = 0; i < depth; ++i)
            out << '\t';
        printNode<KK, VV, CC>(out, *t) << '\n';
        printTree<KK, VV, CC>(t->left, out, depth + 1);
    }
}

template <typename KK, typename VV, typename CC>
void vizTree(
    typename BinarySearchTree<KK, VV, CC>::const_node_ptr node, 
    std::ostream & out,
    typename BinarySearchTree<KK, VV, CC>::const_node_ptr prev = nullptr
) {
    if(node) {
        std::hash<KK> khash{};

        out << "\t" "node_" << (uint32_t) khash(node->element.first)
            << "[label=\"" << node->element.first 
            << " [" << node->element.second << "]\"];" << std::endl;
        
        if(prev)
            out << "\tnode_" << (uint32_t) khash(prev->element.first) <<  " -> ";
        else
            out << "\t";
        
        out << "node_" << (uint32_t) khash(node->element.first) << ";" << std::endl;
    
        vizTree<KK, VV, CC>(node->left, out, node);
        vizTree<KK, VV, CC>(node->right, out, node);
    }
}

template <typename KK, typename VV, typename CC>
void vizTree(
    const BinarySearchTree<KK, VV, CC> & bst, 
    std::ostream & out = std::cout
) {
    out << "digraph Tree {" << std::endl;
    vizTree<KK, VV, CC>(bst._root, out);
    out << "}" << std::endl;
}
