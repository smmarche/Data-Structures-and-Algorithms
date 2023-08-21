#ifndef QUEUE_H
#define QUEUE_H
#include "List.h"

template <typename T, typename Container = List<T>>
class Queue {

    template <typename T1, typename C1>
    friend bool operator==(const Queue<T1, C1>&, const Queue<T1, C1>&);

    public:
        // Aliases for accessing data types outside of the class
        using container_type  = Container;
        using value_type      = typename Container::value_type;
        using size_type       = typename Container::size_type;
        using reference       = typename Container::reference;
        using const_reference = typename Container::const_reference;

    private:
        Container c;

    public:
        // The constructors, destructor, and assignment operators are done for you
        Queue() = default;
        Queue(const Queue& other) = default;
        Queue(Queue&& other) = default;
        ~Queue() = default;
        Queue& operator=(const Queue& other) = default;
        Queue& operator=(Queue&& other) = default;

        reference front() {
            return c.front();
        }
        const_reference front() const {
            return c.front();
        }
        reference back() {
            return c.back();
        }
        const_reference back() const {
            return c.back();
        }

        bool empty() const {
            return c.empty();
        }
        size_type size() const {return c.size(); }

        void push(const value_type& value) {
            c.push_back(value);
        }
        void push(value_type&& value) {
            c.push_back(std::move(value));
        }
        void pop() {
            c.pop_front();
        }
};

template <typename T, typename Container>
inline bool operator==(const Queue<T, Container>& lhs, const Queue<T, Container>& rhs) {
    if(lhs.c.size() != rhs.c.size()){ //check size of queues
        return false;
    }
    auto lhs_iter = lhs.c.begin();
    auto rhs_iter = rhs.c.begin();
    for(int i = 0; i < lhs.c.size(); i++){ //check if each element of the queues are the same
        if(*lhs_iter != *rhs_iter){
            return false;
        }
        lhs_iter++; //move the iterators in the queue along
        rhs_iter++;
    }
    return true;
}

#endif
