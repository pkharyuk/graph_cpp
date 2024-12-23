#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

// Custom implementation of Graph class (dense model)
// Few details:
// - explicitly stores adjacency matrix as 2d array;
// - its dimensions are defined by n_vertices field;
// - each vertex is encoded as unsigned integer from [0, n_vertices-1]
// - (non-)zero entry of adjacency matrix encodes existing (lack of) the edge;

#include "custom_types.hpp"
#include "custom_random.hpp"
#include "handle_input.hpp"
#include "custom_utils.hpp"

template <class W>
class Graph {
    bool is_directed;
    uint n_vertices;
    uint n_edges;
    W **adjacency;

public:
    Graph(uint n_vertices = 0, bool is_directed = false); // constructor
    Graph(const Graph<W>& g); // copy constructor
    // read from file as a conversion constructor
    Graph(const char *fpath, bool is_directed = false);
    ~Graph(); // destructor

    void add_edge(uint u, uint v, W weight);
    void add_edge(uint ind, W weight); // hereinafter: ind = f(u, v)

    void remove_edge(uint u, uint v);
    void remove_edge(uint ind);

    bool is_edge(uint u, uint v) const;
    bool is_edge(uint ind) const;

    bool is_graph_directed() const { return is_directed; };
    uint get_order() const { return n_vertices; }; // number of vertices
    uint get_size() const { return n_edges; }; // number of edges
    void get_vertices(uint **vertices, uint *n) const; // vertices with edges
    void get_neighbours(uint u, uint **neighbours, uint *n) const;
    void get_edges(uint **& edges, W *& weights, uint& n) const;

    W get_edge_value(uint u, uint v) const;
    W get_edge_value(uint ind) const;

    void set_edge_value(
        uint u, uint v, W weight, bool check_exists = true
    );
    void set_edge_value(
        uint ind, W weight, bool check_exists = true
    );

    void save_to_file(const char *fpath) const;

    void print_adjacency(
        const char *weight_format_str,
        uint n_spaces
    ) const;
    void print_edges(const char *weight_format_str) const;

private:
    // ignoring delegation feature
    void init(uint n_vertices, bool is_directed); 
    
    // moving helper functions from static C-like definitions here
    void check_input_weight(W weight) const;
    void check_input_node(uint u) const;
    void ind_to_edge(uint ind, uint& i, uint& j) const;
};

// Sample a random graph with natural weights
Graph<uint>* sample_random_undirected_uint_graph(
    seed_t& seed,
    uint n_vertices,
    double density = 0.1,
    uint min_distance = 1,
    uint max_distance = 1
);

#include "graph.tpp"

#endif

// Author: Pavel Kharyuk, 2024
