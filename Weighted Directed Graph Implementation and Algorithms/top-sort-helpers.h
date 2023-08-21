#pragma once

#include <unordered_map>

#include "weighted-graph.hpp"
#include "graph-types.h"

template <typename T>
void computeIndegrees(const WeightedGraph<T>& graph, std::unordered_map<value_type<T>, int>& indegrees) {
    for(auto it = graph.begin(); it != graph.end(); ++it){ //setting all elements in indegrees to 0
        indegrees[it->first] = 0;
    }
    for(auto it = graph.begin(); it != graph.end(); ++it){ //iterates through the graph
        for(auto adj_it = it->second.begin(); adj_it != it->second.end(); ++adj_it){ //for each vertex in the adjacency list from the current node we are on
            indegrees[adj_it->first] = indegrees[adj_it->first] + 1; //use the key to increment the number of indegrees at that key position
        }
    }
    
}
