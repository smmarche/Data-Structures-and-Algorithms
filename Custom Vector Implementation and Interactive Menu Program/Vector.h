#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::out_of_range
#include <type_traits> // std::is_same

template <class T>
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;

    // function that grows the vector
    void grow() {
        if(_capacity == 0){
            _capacity = 1;
        }
        else{
        _capacity = 2*_capacity;
        }
        T* temparray = new T [_capacity];
        for(int i=0; i < _size; i++){
            temparray[i]=std::move(array[i]);
        }
        delete[] array;
        array = temparray;
    }

public:
    Vector() noexcept :_capacity(0), _size{0}, array(nullptr){}//initialize the data at 0, nullptr
    Vector(size_t count, const T& value):_capacity(count), _size(count), array(new T [count]){//populate the vector with count copies of a value
        for(int i = 0; i < count; i++){//
            array[i]=value;
        }
    }
    explicit Vector(size_t count):_capacity(count),_size(count), array(new T [count]){ //create an array on the heap with elements of T a count number of times
        for(int i = 0; i < count; i++){
            array[i] = T(); 
        }

    }
    Vector(const Vector& other):_capacity(other._capacity), _size(other._size), array(new T [other._capacity]){ //initialize the vector
        for(int i = 0; i < other._size; i++){
            array[i] = other.array[i]; //set the values of array to the values in other's array
        }
    }
    //MOVE CONSTRUCTOR-set everything in the vector = to whatever is in other then wipe other's data
    Vector(Vector&& other) noexcept : _capacity(other._capacity), _size(other._size), array(other.array){
        //this->size = _size this implies Vector array since were in the Vector class
        //set everthing in the Vector object = to others members
        //then delete other's members
        other._capacity = 0;
        other._size = 0;
        other.array = nullptr;
    }
    ~Vector() {
        delete[] array;
        array = nullptr;
        _capacity = 0;
        clear();
    }
    Vector& operator=(const Vector& other) {
        //set everything in array, size, and capacity = 0;
        if(this == &other){//checking if the array is the same
            return *this;
        }
        delete[] array;
        _size = other._size;
        _capacity = other._capacity;
        array = new T [_capacity];
        for(int i=0; i<_size;i++){
            array[i]=other.array[i];
        }
        return *this;    
    }
    Vector& operator=(Vector&& other) noexcept {
        if(this == &other){//checking if the array is the same
            return *this;
        }
        delete[] array;
        _size = other._size;
        _capacity = other._capacity;
        array = other.array;
        other._capacity = 0;
        other._size = 0;
        other.array = nullptr;
        return *this;
    }

    iterator begin() noexcept {
        return iterator(array);
    }
    iterator end() noexcept {
        return iterator(array + _size);
    }

    [[nodiscard]] bool empty() const noexcept {
        return _size==0;
    }
    size_t size() const noexcept {
        return _size;
    }
    size_t capacity() const noexcept {
        return _capacity;
    }
    T& at(size_t pos) {
        if(pos >= _size){
            throw std::out_of_range("");
        }
        return array[pos];
    }
    const T& at(size_t pos) const {
        if(pos >= _size){
            throw std::out_of_range("");
        }
        return array[pos];
    }

    T& operator[](size_t pos) {
        //return a reference to the specified position in the array
        return this->array[pos];
    }
    const T& operator[](size_t pos) const {
        return this->array[pos];
    }
    T& front() {
        return array[0];
    }
    const T& front() const {
        return array[0];
    }
    T& back() {
        return array[_size -1];
    }
    const T& back() const {
        return array[_size -1];
    }

    void push_back(const T& value) {
        while(_size >= _capacity){
            grow();
        }
        array[_size]=value;
        _size++;
    }
    void push_back(T&& value) {
        while(_size >= _capacity){
            grow();
        }
        array[_size]=std::move(value);
        _size++;       
    }
    void pop_back() {
        _size--;
    }

    iterator insert(iterator pos, const T& value) {
        ptrdiff_t insert_index = pos - begin();
        //check if it's in bounds
        if (_size == _capacity) grow();
        //make space shift everything that's there 
        for(ptrdiff_t index = _size; index > insert_index; --index ) //start at the back
        {
            array[index] = std::move(array[index-1]);
        }
        array[insert_index]=value;
        ++_size;
        return begin() + insert_index;
     }

    iterator insert(iterator pos, T&& value) { 
        ptrdiff_t insert_index = pos - begin();
        //check if it's in bounds
        if (_size == _capacity) grow();
        //make space shift everything that's there 
        for(ptrdiff_t index = _size; index > insert_index; --index )//start at the back
        {
            array[index] = std::move(array[index-1]);
        }
        array[insert_index]=std::move(value);
        ++_size;
        return begin() + insert_index;        
    }
    
    iterator insert(iterator pos, size_t count, const T& value) { 
        if (count == 0) return pos;
        ptrdiff_t insert_index = pos - begin();
        while(_size + count > _capacity) grow();//grow multiple times
        //shift from the back
        for (ptrdiff_t index = _size + count - 1; index > insert_index + count -1; --index)//starting at the spot after the index
        {
            array[index]= std::move(array[index-count]);
        }
        for (size_t index = insert_index; index < insert_index + count; ++index){
            array[index]=value;
        }
        _size += count;
        return begin() + insert_index;
     }
    iterator erase(iterator pos) {
        if(pos == end()){
            pop_back();
            return end();            
        }
        for(ptrdiff_t i= pos - begin(); i < end()-begin()-1; i++){
            array[i]= array[i+1];
        }
        _size-=1;
        return pos;
    }
    iterator erase(iterator first, iterator last) {
        size_t difference = last - first;
        if(last == end()){
            _size = _size - difference;
            return end();
        }
        for(size_t i = first - begin(); i < _size - difference; i++ ){
            array[i]=array[i+difference];
        }
        _size -= difference;
        return first;
    }

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    private:
        pointer ptr;

    public:
        iterator() :ptr(nullptr) {}
        // constructors
        //copy constructor
        iterator(pointer some_pointer) :ptr(some_pointer){}
        // This assignment operator
        iterator& operator=(const iterator&) noexcept = default;

        [[nodiscard]] reference operator*() const noexcept {
            return *ptr;
        }
        [[nodiscard]] pointer operator->() const noexcept {
            return ptr;
        }

        // Prefix Increment: ++a
        iterator& operator++() noexcept {
            ++ptr;
            return *this;//return address of thing the pointer points to
        }
        // Postfix Increment: a++
        iterator operator++(int) noexcept {
            iterator temporary = *this;
            ++(*this);
            return temporary;
        }
        // Prefix Decrement: --a
        iterator& operator--() noexcept {
            --ptr;
            return *this;
        }
        // Postfix Decrement: a--
        iterator operator--(int) noexcept {
            iterator temporary = *this;
            --(*this);
            return temporary;
        }

        iterator& operator+=(difference_type offset) noexcept {
            ptr+=offset;
            return *this;
        }
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept {
            iterator temporary = *this;
            return temporary+=offset;
        }
        
        iterator& operator-=(difference_type offset) noexcept {
            ptr-=offset;
            return *this;
        }
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept {
            iterator temporary = *this;
            return temporary-=offset;
        }
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept {
            return ptr-rhs.ptr;
        }

        [[nodiscard]] reference operator[](difference_type offset) const noexcept {
            return ptr[offset];
        }

        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept {
            return (ptr == rhs.ptr);
        }
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept {
            return (ptr != rhs.ptr);
        }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept {
            return(ptr < rhs.ptr);
        }
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept {
            return (ptr > rhs.ptr);
        }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept {
            return (ptr <= rhs.ptr);
        }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept {
            return (ptr >= rhs.ptr);
        }
    };


    void clear() noexcept {
        _size = 0;
    }
};

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace {
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const& iterator) noexcept {
    return (iterator+offset);
}

#endif
