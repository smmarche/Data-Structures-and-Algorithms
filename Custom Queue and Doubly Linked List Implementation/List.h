#pragma once

#include <cstddef> // size_t
#include <iterator> // std::bidirectional_iterator_tag
#include <type_traits> // std::is_same, std::enable_if

template <class T>
class List {
    private:
    struct Node {
        Node *next, *prev;
        T data;
        explicit Node(Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev} {}
        explicit Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{data} {}
        explicit Node(T&& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{std::move(data)} {}
    };

    template <typename pointer_type, typename reference_type>
    class basic_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = pointer_type;
        using reference         = reference_type;
    private:
        friend class List<value_type>;
        using Node = typename List<value_type>::Node;

        Node* node;

        explicit basic_iterator(Node* ptr) noexcept : node{ptr} {}
        explicit basic_iterator(const Node* ptr) noexcept : node{const_cast<Node*>(ptr)} {}

    public:
        basic_iterator() : node(nullptr) {};
        basic_iterator(const basic_iterator&) = default;
        basic_iterator(basic_iterator&&) = default;
        ~basic_iterator() = default;
        basic_iterator& operator=(const basic_iterator&) = default;
        basic_iterator& operator=(basic_iterator&&) = default;

        reference operator*() const {
            //returning a reference to the node's data
            return node->data;
        }
        pointer operator->() const {
            //return a pointer (the address) of the data in the node
            return &(node->data);
        }

        // Prefix Increment: ++a
        basic_iterator& operator++() {
            /* move the iterator to the next node and return
            an address to the iterator */
            node=node->next;
            return *this;
        }
        // Postfix Increment: a++
        basic_iterator operator++(int) {
        // Postfix Increment: a++

            Node* temp_node = node;
            node = node->next;
            return basic_iterator(temp_node);
        }
        // Prefix Decrement: --a
        basic_iterator& operator--() {
            //iterator points to prior node and returns a reference
            node=node->prev;
            return *this;
        }
        // Postfix Decrement: a--
        basic_iterator operator--(int) {
            Node* temp_node = node;
            node = node->prev;
            return basic_iterator(temp_node);
        }

        bool operator==(const basic_iterator& other) const noexcept {
            //return t/f if the iterators point to the same thing
            return node == other.node;
        }
        bool operator!=(const basic_iterator& other) const noexcept {
            // return t/f if they are not equal
            return node != other.node;
        }
    };

public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = basic_iterator<pointer, reference>;
    using const_iterator  = basic_iterator<const_pointer, const_reference>;

private:
    Node head, tail;
    size_type _size;

public:
    List(): head(), tail(), _size(0){ //the list initialier
        //head points to tail and tail points to head
        head.next=&tail;
        tail.prev=&head;
    }
    List( size_type count, const T& value ): head(), tail(), _size(0){
        head.next=&tail;
        tail.prev=&head;
        for(size_type i=count; i > 0; i--){
            push_back(value);
        }
    }
    explicit List( size_type count ) : head(), tail(), _size(0){
        head.next=&tail;
        tail.prev=&head;
        for(size_type i=count; i > 0; i--){
            push_back(T());
        }
    }
    List( const List& other ): head(), tail(), _size(0) {
        head.next=&tail;
        tail.prev=&head;
        basic_iterator iter = other.begin();
        for (iter; iter!= other.end(); iter++){
            push_back(*iter);
        }
    }
    List( List&& other ) : head(), tail(), _size(0) {

        if(other._size == 0){
            head.next=&tail;
            tail.prev=&head;            
        }
        else{
            //point head/tail to other's
            head.next= other.head.next;
            tail.prev=other.tail.prev;
            // others first and last elements point to this head and tail
            other.head.next->prev = &head;
            other.tail.prev->next = &tail;
            //head and tail point to each other
            other.head.next = &(other.tail);
            other.tail.prev = &(other.head);
            
            _size = other._size;
            other._size = 0;
        }

    }
    ~List() {
        clear();
        //_size=0;
    }
    List& operator=( const List& other ) {
        // TODO
        if(this != &other){
            clear();
            head.next=&tail;
            tail.prev=&head;
            //_size = other.size();
            //iterator other1 (other.head.next);
            basic_iterator iter = other.begin();
            for (iter; iter!= other.end(); iter++){
                push_back(*iter);
            }
        }
        return *this;
    }

    List& operator=( List&& other ) noexcept {
        // TODO
        if(this != &other){
            clear();
            head.next=&tail;
            tail.prev=&head;
            _size = other.size();
            if(_size != 0){
                head.next = other.head.next;
                tail.prev = other.tail.prev;
                head.next->prev = &head;
                tail.prev->next= &tail;
                other.head.next = &(other.tail);
                other.tail.prev = &(other.head);
                other._size = 0;
            }
        }
        return *this;
    }

    reference front() {
        return head.next->data;
    }
    const_reference front() const {

        return const_reference(head.next->data);
    }
	
    reference back() {
        return tail.prev->data;
    }
    const_reference back() const {
        return const_reference(tail.prev->data);
    }
	
    iterator begin() noexcept {
        if(empty()){
            return iterator(&tail);
        }
        return iterator(head.next);

    }
    const_iterator begin() const noexcept {
        if(empty()){
            return const_iterator(&tail);
        }
        return const_iterator(head.next);
    }
    const_iterator cbegin() const noexcept {
        return const_iterator(head.next);
    }

    iterator end() noexcept {
        return iterator(&tail);
    }
    const_iterator end() const noexcept {
        return const_iterator(&tail);
    }
    const_iterator cend() const noexcept {
        return const_iterator(&tail);
    }

    bool empty() const noexcept {
        // return t if empty
        return size()==0;
    }

    size_type size() const noexcept {
        // return no. of elements
        return _size;
    }

    void clear() noexcept {
        //for(auto i = _size; i > 0; i--){//while not empty pop_back
        while(!empty()){
            pop_back();
        }
    }

    iterator insert( const_iterator pos, const T& value ) {
        //insert a value before pos
        //create a new node and put value in it
        Node* new_node = new Node(value);
        //set new_node pointers

        new_node->next=pos.node;//point new_node next to the node at pos
        new_node->prev=pos.node->prev;//point new_node prev to the node behind pos
        //update the nodes already in the list's pointers
        //pos.node->prev accesses the node behind pos node
        pos.node->prev->next=new_node;//point prev node's next ptr to new node
        pos.node->prev = new_node;//node at pos prev ptr should point to new node
        _size++;
        //return iterator to the new node
        return iterator(new_node);
    }
    iterator insert( const_iterator pos, T&& value ) {
        Node* new_node = new Node(std::move(value));
        new_node->next=pos.node;//point new_node next to the node at pos
        new_node->prev=pos.node->prev;//point new_node prev to the node behind pos
        //update the nodes already in the list's pointers
        //pos.node->prev accesses the node behind pos node
        pos.node->prev->next=new_node;//point prev node ptr to new node
        pos.node->prev = new_node;//node at pos prev ptr should point to new node
        _size++;
        //return iterator to the new node
        return iterator(new_node);
    }

    iterator erase( const_iterator pos ) {
    //remove element at pos
        pos.node->prev->next=pos.node->next;//gets the next pointer of the node in front of pos and points it toward the node after pos
        pos.node->next->prev=pos.node->prev;//gets the prev pointer of the node after pos and points that to the not before pos
        iterator temp = iterator(pos.node->next);
        delete pos.node;//deallocate memory for node at pos
        _size-=1;
        return temp;
    }

    void push_back( const T& value ) {
        //add value to the back of the list
        insert(const_iterator(&tail),value);//insert at the end
    }
    void push_back( T&& value ) {
        //add value to back of the list
        insert(const_iterator(&tail), std::move(value));
    }

    void pop_back() {
        erase(--end());
    }
	
    void push_front( const T& value ) {
        insert(cbegin(), value);
    }
	void push_front( T&& value ) {
        insert(cbegin(), std::move(value));
    }

    void pop_front() {
        erase(cbegin());
    }

    /*
      You do not need to modify these methods!
      
      These method provide the non-const complement 
      for the const_iterator methods provided above.
    */
    iterator insert( iterator pos, const T & value) { 
        return insert((const_iterator &) (pos), value);
    }

    iterator insert( iterator pos, T && value ) {
        return insert((const_iterator &) (pos), std::move(value));
    }

    iterator erase( iterator pos ) {
        return erase((const_iterator&)(pos));
    }
};


/*
    You do not need to modify these methods!

    These method provide a overload to compare const and 
    non-const iterators safely.
*/
 
namespace {
    template<typename Iter, typename ConstIter, typename T>
    using enable_for_list_iters = typename std::enable_if<
        std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::iterator, 
            Iter
        >{} && std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::const_iterator,
            ConstIter
        >{}, T>::type;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) == rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) == lhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) != rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) != lhs;
}
