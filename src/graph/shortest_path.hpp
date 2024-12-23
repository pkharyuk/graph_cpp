#ifndef __SHORTEST_PATH_HPP__
#define __SHORTEST_PATH_HPP__

#include "custom_types.hpp"
#include "graph.hpp"
#include "min_heap.hpp"

// leetcode experience in action!

// Supporting structure (=class with public methods by default)
// stores computational results
template <class W>
struct ShortestPathResults {
    uint n_nodes; // number of nodes
    int src_node; // source node 
    int dst_node; // destination node
    uint *path;    // src -> dst path, 1d array
    uint path_len; // length of the path

    ShortestPathResults(uint n = 0, int src = -1, int dst = -1)
        : n_nodes(n)
        , src_node(src)
        , dst_node(dst)
        , path(0)
        , path_len(0)
        {}

    virtual ~ShortestPathResults()
        { flush(); }

    // common for all ancestors
    uint get_shortest_path_len(
        int src_node = -1, int dst_node = -1
    ) const;

    // pure virtual
    virtual W get_shortest_path_cost(
        int src_node = -1, int dst_node = -1
    ) const = 0;
    virtual void extract_shortest_path(
        int src_node = -1, int dst_node = -1
    ) = 0;
    virtual bool is_node_reachable(
        uint dst_node, int src_node = -1
    ) const = 0;

private:
    // common for all ancestors
    void flush(); // virtual destructor, but non-virtual flush

protected:
    void base_flush()
        { flush(); }
    void check_node(int input_node, const char *which_node) const;
    void check_src_dst_nodes(int input_src_node, int input_dst_node) const;
    void shortest_path_extractor(
        uint src_node, uint dst_node, uint *preds
    );
};

// Supporting structure for Dijkstra algorithm
// Can be used for a specified (src, dst) pair of nodes
// or for a single src_node
template <class W>
struct SingleSourceResults : public ShortestPathResults<W> {
    uint *preds; // predecessors, 1d array
    W *dists; // distances, 1d array
    bool all_nodes_visited; // whether stoped early or not

    SingleSourceResults(uint n = 0, int src = -1, int dst = -1)
        : ShortestPathResults<W>(n, src, dst)
        , preds(0)
        , dists(0)
        , all_nodes_visited(false)
        {}

    ~SingleSourceResults()
        { flush(); }

    W get_shortest_path_cost(int src_node = -1, int dst_node = -1) const;
    void extract_shortest_path(int src_node = -1, int dst_node = -1);
    bool is_node_reachable(uint dst_node, int src_node = -1) const;

private:
    void flush();
    void check_single_source_results_inputs(
        int& input_src_node, int& input_dst_node
    ) const;

};

// Supporting structure for Floyd-Warshall algorithm
// Can be used for every (src, dst) pair of nodes
template <class W>
struct FloydWarshallResults : public ShortestPathResults<W> {
    uint **preds; // predecessors, 2d array
    W **dists; // distances, 2d array

    FloydWarshallResults(uint n = 0, int src = -1, int dst = -1)
        : ShortestPathResults<W>(n, src, dst)
        , preds(0)
        , dists(0)
        {}

    ~FloydWarshallResults()
        { flush(); }

    W get_shortest_path_cost(int src_node = -1, int dst_node = -1) const;
    void extract_shortest_path(int src_node = -1, int dst_node = -1);
    bool is_node_reachable(uint dst_node, int src_node) const;

private:
    void flush();
    void check_floyd_warshall_results_inputs(
        int input_src_node, int input_dst_node
    ) const;

};


enum ShortestPathAlgorithm {
    dijkstra = 0,
    floyd_warshall,
    bellman_ford,
    n_shortest_path_algs
};

// Main static class
// can be useful if we consider different algorithms
template <class W>
class ShortestPath {
public:
    
    static ShortestPathResults<W> *find_shortest_path(
        const Graph<W>& g,
        uint src_node,
        int dst_node = -1,
        enum ShortestPathAlgorithm code = ShortestPathAlgorithm::dijkstra
    );
    // length = number of nodes coposing the path
    // note: may be high but result in a smaller/equal cost
    static double compute_average_shortest_path_length_for_node(
	    const Graph<W>& g,
        uint src_node,
        ShortestPathResults<W> *shortest_path_results = 0,
        enum ShortestPathAlgorithm code = ShortestPathAlgorithm::dijkstra
    );
    // cost = sum of edge values
    static double compute_average_shortest_path_cost_for_node(
	    const Graph<W>& g,
        uint src_node,
        ShortestPathResults<W> *shortest_path_results = 0,
        enum ShortestPathAlgorithm code = ShortestPathAlgorithm::dijkstra
    );
private:
    static void dijkstra(
        const Graph<W> &g,
        uint src_node,
        int dst_node,
        SingleSourceResults<W> *&results,
        bool visit_all = false);
    static void floyd_warshall(
        const Graph<W>& g,
        FloydWarshallResults<W> *& results
    );
    static void bellman_ford(
        const Graph<W>& g,
        uint src_node,
        int dst_node,
        SingleSourceResults<W> *& results
    );
};

#include "shortest_path.tpp"

#endif

// Author: Pavel Kharyuk, 2024