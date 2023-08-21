#include <cstddef>    // size_t
#include <functional> // std::hash
#include <ios>
#include <utility>    // std::pair
#include <iostream>

#include "primes.h"



template <typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>>
class UnorderedMap {
    public:

    using key_type = Key;
    using mapped_type = T;
    using const_mapped_type = const T;
    using hasher = Hash;
    using key_equal = Pred;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    private:

    struct HashNode {
        HashNode *next;
        value_type val;

        HashNode(HashNode *next = nullptr) : next{next} {}
        HashNode(const value_type & val, HashNode * next = nullptr) : next { next }, val { val } { }
        HashNode(value_type && val, HashNode * next = nullptr) : next { next }, val { std::move(val) } { }
    };

    size_type _bucket_count;
    HashNode **_buckets;

    HashNode * _head;
    size_type _size;

    Hash _hash;
    key_equal _equal;

    static size_type _range_hash(size_type hash_code, size_type bucket_count) {
        return hash_code % bucket_count;
    }

    public:

    template <typename pointer_type, typename reference_type, typename _value_type>
    class basic_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = _value_type;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;
        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        const UnorderedMap * _map;
        HashNode * _ptr;

        explicit basic_iterator(UnorderedMap const * map, HashNode *ptr) noexcept {
            _map = map;
            _ptr = ptr;
        }

    public:
        basic_iterator():_map(nullptr), _ptr(nullptr) {};

        basic_iterator(const basic_iterator &) = default;
        basic_iterator(basic_iterator &&) = default;
        ~basic_iterator() = default;
        basic_iterator &operator=(const basic_iterator &) = default;
        basic_iterator &operator=(basic_iterator &&) = default;
        reference operator*() const {return _ptr->val;}
        pointer operator->() const {return &(_ptr->val);}
        basic_iterator &operator++() {
            size_type i = _map->_bucket(_ptr->val.first);
            if(_ptr->next==nullptr){
                i++;
                while(i < _map->_bucket_count && _map->_buckets[i] == nullptr) i++;
                if (i == _map->_bucket_count) _ptr = nullptr;
                else _ptr = _map->_buckets[i];
            } else _ptr = _ptr->next;
            return *this;
        }
        basic_iterator operator++(int) {
            basic_iterator temp = *this;
            if (_ptr->next == nullptr) {
                size_t i = _map->_bucket(_ptr->val.first) + 1;
                while(i < _map->_bucket_count && _map->_buckets[i] == nullptr) i++;
                if(i == _map->_bucket_count) _ptr = nullptr;
                else _ptr = _map->_buckets[i];
            } else _ptr = _ptr->next;
            return temp;
        }
        bool operator==(const basic_iterator &other) const noexcept {
            return _ptr == other._ptr;
        }
        bool operator!=(const basic_iterator &other) const noexcept {
            return _ptr != other._ptr;
        }
    };

    using iterator = basic_iterator<pointer, reference, value_type>;
    using const_iterator = basic_iterator<const_pointer, const_reference, const value_type>;

    class local_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<const key_type, mapped_type>;
            using difference_type = ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

        private:
            friend class UnorderedMap<Key, T, Hash, key_equal>;
            using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

            HashNode * _node;

            explicit local_iterator( HashNode * node ) noexcept :_node(node) {}

        public:
            local_iterator() {
                _node = nullptr;
            }

            local_iterator(const local_iterator &) = default;
            local_iterator(local_iterator &&) = default;
            ~local_iterator() = default;
            local_iterator &operator=(const local_iterator &) = default;
            local_iterator &operator=(local_iterator &&) = default;
            reference operator*() const {
                return _node->val;
            }
            pointer operator->() const {
                return &(_node->val);
            }
            local_iterator & operator++() {
                _node = _node->next;
                return *this;
            }
            local_iterator operator++(int) {
                local_iterator prev = local_iterator(_node);
                _node = _node->next;
                return prev;
            }

            bool operator==(const local_iterator &other) const noexcept {
                return _node ==other._node;
            }
            bool operator!=(const local_iterator &other) const noexcept {
                return _node !=other._node;
            }
    };

private:

    size_type _bucket(size_t code) const {
        return _range_hash(code, _bucket_count);
    }
    size_type _bucket(const Key & key) const {
        return _bucket(_hash(key));
    }
    size_type _bucket(const value_type & val) const {
        return _bucket(val.first);
    }

    HashNode*& _find(size_type code, size_type bucket, const Key & key) {
        HashNode** node = &(_buckets[bucket]);
        while(*node != nullptr){
            if(_equal(key,(*node)->val.first)){
                return *node;
            }
            node = &((*node)->next);
        }
        return *node;
    }

    HashNode*& _find(const Key & key) {
        return _find(_hash(key),_bucket(key), key);
    }

    HashNode * _insert_into_bucket(size_type bucket, value_type && value) {
        HashNode* newNode = new HashNode(std::move(value), _buckets[bucket]);
        _buckets[bucket] = newNode;
        Key k = newNode->val.first;
        if(_head==nullptr||_bucket(k)<= _bucket(_head->val)){
            _head = _buckets[bucket];
        }
        _size+=1;
        return newNode;
    }

    void _move_content(UnorderedMap & src, UnorderedMap & dst) {
        dst._bucket_count = src._bucket_count;
        dst._buckets = src._buckets;
        dst._hash = src._hash;
        dst._equal = src._equal;
        dst._size = src._size;
        dst._head = src._head;
        src._size = 0;
        src._head = nullptr;
        src._buckets = new HashNode* [src._bucket_count]();
    }

public:
    explicit UnorderedMap(size_type bucket_count, const Hash & hash = Hash { },
                const key_equal & equal = key_equal { }) {
                    _bucket_count = next_greater_prime(bucket_count);
                    _buckets = new HashNode* [_bucket_count]();
                    _hash = hash;
                    _equal = equal;
                    _size = 0;
                    _head = nullptr;
                }

    ~UnorderedMap() {
        clear();
        delete[] _buckets;
        _buckets = nullptr;
    }

    UnorderedMap(const UnorderedMap & other) {
        _bucket_count = other._bucket_count;
        _buckets = new HashNode*[_bucket_count]();
        _hash = other._hash;
        _equal = other._equal;
        _head = nullptr;
        _size = 0;
        const_iterator iter = other.cbegin();
        while(iter != cend()){
            insert(iter._ptr->val);
            iter++;
        }
    }

    UnorderedMap(UnorderedMap && other) {
        _move_content(other, *this);
    }

    UnorderedMap & operator=(const UnorderedMap & other) {//Copy Assignment Operator

        if(this == &other){
            return *this;
        }
        clear();
        delete[] _buckets;
        _buckets = nullptr;
        _bucket_count = other._bucket_count;
        _buckets = new HashNode* [_bucket_count]();
        _hash = other._hash;
        _equal = other._equal;
        _head = other._head;
        _size = 0;
        const_iterator iter = other.cbegin();
        while(iter != cend()){
            insert(iter._ptr->val);
            iter++;
        }
    }

    UnorderedMap & operator=(UnorderedMap && other) {//Move assignment Operator
        if(this == &other){
            return *this;
        }
        clear();
        delete [] _buckets;
        _buckets = nullptr;
        _move_content(other, *this);
    }

    void clear() noexcept {
        size_t temp_size = size();
        iterator clr = begin();
        for(size_t i = 0; i < temp_size; i++){
            clr = erase(clr);
        }
        _head = nullptr;
    }

    size_type size() const noexcept { return _size;}

    bool empty() const noexcept {return _size == 0;}

    size_type bucket_count() const noexcept {return _bucket_count;}

    iterator begin() {
        return iterator(this,_head);
    }
    iterator end() {
        return iterator(this,nullptr);
    }

    const_iterator cbegin() const {
        return const_iterator(this,_head);
        }
    const_iterator cend() const {
        return const_iterator(this,nullptr);
    };

    local_iterator begin(size_type n) {
        return local_iterator(_buckets[n]);
    }
    local_iterator end(size_type n) {
        return local_iterator(nullptr);
    }

    size_type bucket_size(size_type n) {
        size_t count = 0;
        HashNode**node = &(_buckets[n]);
        while(*node !=nullptr){
            count+=1;
            node = &((*node)->next);
        }
        return count;
    }

    float load_factor() const {
        return (float)size()/_bucket_count;
    }

    size_type bucket(const Key & key) const {
        return _range_hash(_hash(key), _bucket_count);
    }

    std::pair<iterator, bool> insert(value_type && value) {
        std::pair<iterator,bool> pair;
        size_type index = _bucket(value.first);
        if(_find(value.first) != nullptr) pair = {iterator(this,_find(value.first)), false};
        else pair = {iterator(this,_insert_into_bucket(index, std::move(value))), true};
        return pair;
    }

    std::pair<iterator, bool> insert(const value_type & value) {

        std::pair<iterator,bool> pair;
        size_type index = _bucket(value.first);
        if(_find(value.first) != nullptr) pair = {iterator(this,_find(value.first)), false};
        else {
            value_type copyvalue = value;
            pair = {iterator(this,_insert_into_bucket(index, std::move(copyvalue))), true};
        }
        return pair;
    }




    T& operator[](const Key & key) {
        if(_find(key) == nullptr){
            std::pair<iterator, bool> val= insert({key,{}});
            return val.first._ptr->val.second;
        }
        else{
            return _find(key)->val.second;
        }
    }

    iterator erase(iterator pos) {
        if (pos == end()){
            return pos;
        }
        iterator temporary = pos;
        temporary++;
        erase(pos._ptr->val.first);
        return temporary;
    }

    size_type erase(const Key & key) {
         iterator temp = begin();
        HashNode* ptr = _find(key);
        if (ptr == nullptr) return 0;
        else if (ptr == _head) {
            temp++;
            _head = temp._ptr;
        }
        if (ptr == _buckets[_bucket(key)]) _buckets[_bucket(key)] = ptr->next;
        else {
            local_iterator ltemp = begin(_bucket(key));
            while (key != ltemp._node->next->val.first) ltemp++;
            ltemp._node->next = ptr->next;
        }
        delete ptr;
        _size--;
        return 1;

    }

    template<typename KK, typename VV>
    friend void print_map(const UnorderedMap<KK, VV> & map, std::ostream & os);
};

template<typename K, typename V>
void print_map(const UnorderedMap<K, V> & map, std::ostream & os = std::cout) {
    using size_type = typename UnorderedMap<K, V>::size_type;
    using HashNode = typename UnorderedMap<K, V>::HashNode;

    for(size_type bucket = 0; bucket < map.bucket_count(); bucket++) {
        os << bucket << ": ";

        HashNode const * node = map._buckets[bucket];

        while(node) {
            os << "(" << node->val.first << ", " << node->val.second << ") ";
            node = node->next;
        }

        os << std::endl;
    }
}
