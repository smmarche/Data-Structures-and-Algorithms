This repository contains C++ implementations of a custom queue and a doubly linked list, along with their respective header files queue.h and list.h. These data structures are designed to provide essential functionality while serving as educational examples of data structure implementation.
Contents

    Usage
    Custom Queue
    Doubly Linked List


Usage

To use these data structures in your C++ project, you can simply include the appropriate header files in your code:
#include "queue.h"
#include "list.h"

int main() {
    // Your code here
    return 0;
}
Custom Queue

The queue.h header file defines a templated Queue class that provides the basic functionalities of a queue, such as push, pop, front, back, empty, and size. The queue class can be customized with different underlying containers and provides an equality operator for comparing queues.
Doubly Linked List

The list.h header file defines a templated List class that implements a doubly linked list. It supports bidirectional iteration, insertion, and removal of elements at various positions, as well as querying front and back elements. The implementation includes separate iterator classes for const and non-const access.
