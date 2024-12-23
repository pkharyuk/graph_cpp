#ifndef __MINIMUM_SPANNING_TREE_HPP__
#define __MINIMUM_SPANNING_TREE_HPP__

#include "graph.hpp"
#include "custom_random.hpp"
#include "min_heap.hpp"
#include "union_find.hpp"
#include "custom_sort.hpp"

// workaround to make a templated type definition
template <class W>
struct GreedyAlg {
    typedef void (*greedy_alg)(seed_t&, const Graph<W>&, Graph<W>&, bool&);
};

template <class W>
class MinSpanningTree {
    
public:
    static bool compute_mst_prim(seed_t& seed, const Graph<W>& g, Graph<W> **t);
    static bool compute_mst_kruskal(seed_t& seed, const Graph<W>& g, Graph<W> **t);

private:
    static bool compute_greedy(
        seed_t& seed, const Graph<W>& g, Graph<W> **t,
        typename GreedyAlg<W>::greedy_alg alg
    );
    static void prim(seed_t& seed, const Graph<W>& g, Graph<W>& h, bool& success);
    static void kruskal(seed_t& seed, const Graph<W>& g, Graph<W>& h, bool& success);
};

#include "minimum_spanning_tree.tpp"

#endif