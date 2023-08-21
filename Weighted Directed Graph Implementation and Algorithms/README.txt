# Weighted Directed Graph Implementation and Algorithms

This repository contains a C++ implementation of a weighted directed graph data structure along with implementations of graph algorithms like Dijkstra's algorithm and topological sorting. The code is organized into multiple header and source files to promote modularity and readability.

## Files

### `weighted-graph.hpp`

This header file defines the `WeightedGraph` class, which represents a directed, weighted graph data structure. It provides functions to add and remove vertices and edges, access adjacency lists, and perform various operations on the graph.

### `graph-types.h`

This header file defines type aliases that simplify the usage of template types in the `WeightedGraph` class. It defines aliases for vertex types, edge types, size types, and iterators.

### `dijkstras-helpers.h`

This header file contains helper functions related to Dijkstra's algorithm. It defines functions like `relax`, `infinity`, `initializeSingleSource`, `DijkstraComparator`, and `updateHeap`. These functions are used in the implementation of Dijkstra's algorithm.

### `top-sort-helpers.h`

This header file contains helper functions related to topological sorting. It defines functions like `computeIndegrees` to calculate indegrees of vertices for topological sorting.

### `graph-algorithms.h`

This header file contains implementations of graph algorithms. It includes the `dijkstrasAlgorithm` function for finding the shortest path using Dijkstra's algorithm and the `topologicalSort` function for performing topological sorting.

### `main.cpp`

This is the main program file that utilizes the graph data structure and algorithms. It reads a graph, performs Dijkstra's algorithm to find the shortest path, and calculates a topological sorting of the graph. The program interacts with the user to input vertices and displays the results.

## Compilation and Usage

To compile the program, ensure that all header and source files are included in the compilation command. For example:

```shell
g++ main.cpp -o graph_program
```

To run the compiled program:

```shell
./graph_program
```

Follow the prompts to input vertices and view the results of Dijkstra's algorithm and topological sorting.
To run the compiled program:
./graph_program
Follow the prompts to input vertices and view the results of Dijkstra's algorithm and topological sorting.
