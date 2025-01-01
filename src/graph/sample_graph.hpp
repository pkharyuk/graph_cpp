#ifndef __SAMPLE_GRAPH_HPP__
#define __SAMPLE_GRAPH_HPP__

#include <stdlib.h>
#include <stdio.h>
#include "custom_types.hpp"
#include "graph.hpp"

// Sample a random graph with natural weights
Graph<uint>* sample_random_undirected_uint_graph(
    seed_t& seed,
    uint n_vertices,
    double density = 0.1,
    uint min_distance = 1,
    uint max_distance = 1
);

#endif