Priority Queue Implementation
This repository contains a C++ implementation of a priority queue data structure using a max heap. The implementation is split into two files: main.cpp and PriorityQueue.h.
Contents

    PriorityQueue.h: This file contains the implementation of the PriorityQueue class template. The class provides functionality for inserting elements with priorities and extracting the element with the highest priority.

    main.cpp: This file showcases the usage of the PriorityQueue class by creating instances of max and min priority queues and demonstrating their behavior.

Usage

    Compile the Program: Compile the main.cpp file using a C++ compiler (such as g++). For example:
g++ -o main main.cpp
Run the Program: Execute the compiled binary:
./main
    This will run the program and demonstrate how the max priority queue works, printing the elements in descending order, and then how the min priority queue works, printing the elements in ascending order.

Customization

You can customize the priority queue implementation by modifying the PriorityQueue class in PriorityQueue.h. The class allows you to define the element type, underlying container type, and comparison function.

    To change the element type, modify the T template parameter.
    To use a different container type (e.g., std::deque), modify the Container template parameter.
    To create a min priority queue or use a different comparison function, modify the Compare template parameter.

Implementation Details

The PriorityQueue class implements a max heap-based priority queue. The implementation includes functions for inserting elements (push), extracting the top element (pop), checking the top element (top), checking if the priority queue is empty (empty), and determining the number of elements in the queue (size).

