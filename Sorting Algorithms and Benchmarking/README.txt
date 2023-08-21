Sorting Algorithms and Benchmarking

This repository contains C++ code for implementing three common sorting algorithms (Bubble Sort, Insertion Sort, and Selection Sort) and a benchmarking tool to measure their performance on various datasets.
Table of Contents

    Introduction
    Files
    Sorting Algorithms
    Benchmarking
    Usage



Introduction

This project showcases the implementation of three fundamental sorting algorithms and a benchmarking tool to evaluate their efficiency in terms of execution time and comparisons made during sorting. The code is designed to be illustrative and educational, demonstrating the basic concepts of these algorithms.
Files

    sorting.h: Contains the implementations of three sorting algorithms: Bubble Sort, Insertion Sort, and Selection Sort.
    main.cpp: Implements the benchmarking tool that measures the performance of sorting algorithms on various datasets.

Sorting Algorithms
Bubble Sort

Bubble Sort is a simple sorting algorithm that repeatedly steps through the list, compares adjacent elements, and swaps them if they are in the wrong order.
Insertion Sort

Insertion Sort builds the final sorted array one item at a time by inserting elements into their correct positions within the sorted portion of the array.
Selection Sort

Selection Sort repeatedly finds the minimum element from the unsorted part of the array and places it at the beginning of the sorted portion.
Benchmarking

The benchmarking tool measures the performance of the sorting algorithms on various datasets. It calculates the execution time and the number of comparisons made during sorting. The tool can accept input data from both files and manual entry.
Usage

    Compile the sorting.h and main.cpp code using a C++ compiler.
    Run the compiled executable for the benchmarking tool to measure the performance of the sorting algorithms on different datasets.

To compile and run the benchmarking tool:
g++ -o sorting_benchmark main.cpp
./sorting_benchmark
