#include "minimum_spanning_tree.hpp"

template <class W>
bool MinSpanningTree<W>::compute_greedy(
    seed_t& seed,
    const Graph<W>& g,
    Graph<W> **t,
    typename GreedyAlg<W>::greedy_alg alg
)
{
    *t = new Graph<W>(g.get_order(), g.is_graph_directed());
    bool success;
    alg(seed, g, **t, success);
    if (!success) {
        delete *t;
        *t = 0;
    }
    return success;
}
    
template <class W>
bool MinSpanningTree<W>::compute_mst_prim(
    seed_t& seed, const Graph<W>& g, Graph<W> **t
)
{
    return compute_greedy(seed, g, t, prim);
}

template <class W>
bool MinSpanningTree<W>::compute_mst_kruskal(
    seed_t& seed, const Graph<W>& g, Graph<W> **t
)
{
    return compute_greedy(seed, g, t, kruskal);
}

template <class W>
struct PrimResults {
    uint n_nodes;
    uint *nodes;
    uint *preds;
    bool success;

    PrimResults()
        : n_nodes(0)
        , nodes(0)
        , preds(0)
        , success(false)
        {}

    ~PrimResults() {
        if (nodes)
            delete[] nodes;
        if (preds)
            delete[] preds;
    }

    void extract_mst(const Graph<W>& g, Graph<W>& t);

};

template <class W>
void PrimResults<W>::extract_mst(const Graph<W>& g, Graph<W>& t)
{
    if (!this->success) {
        fprintf(
            stderr,
            "Attempted to extract MST from unsuccessful computations\n"
        );
        return;
    }
    for (uint i = 0; i < this->n_nodes; i++) {
        uint v = this->nodes[i];
        uint u = this->preds[i];
        if (u == v)
            continue;
        W w = g.get_edge_value(u, v);
        t.add_edge(u, v, w);
    }
}

// current implementation can not handle
// the case with non-dense nodes (e.g., if
// there are nodes 0, 1, 3, 4, 5 and no 2).
template <class W>
static void prim_procedure(
    seed_t& seed, const Graph<W>& g, Graph<W>& h, PrimResults<W>& results
)
{
    g.get_vertices(&results.nodes, &results.n_nodes);
    results.preds = new uint[results.n_nodes];
    uint n_vertices = g.get_order();
    uint *nodes = new uint[n_vertices];
    uint *preds = new uint[n_vertices];
    W *dists = new W[n_vertices];
    bool *chosen = new bool[n_vertices]();
    for (uint k = 0; k < n_vertices; k++) {
        nodes[k] = n_vertices; // mark nodes w/o edges
        preds[k] = n_vertices;
        dists[k] = infinity;
    }
    for (uint k = 0; k < results.n_nodes; k++) {
        uint ind = results.nodes[k];
        nodes[ind] = ind;
        preds[ind] = ind;
        //printf("%u\n", ind);
    }
    uint src_node = n_vertices;
    do {
        src_node = random_integer_draw(seed, 0, n_vertices-1);
    } while (nodes[src_node] == n_vertices);
    dists[src_node] = 0;
    MinHeap<uint, W, uint> heap;
    QueueElement<uint, W> *qe = new QueueElement<uint, W>(
        &nodes[src_node], dists[src_node]
    );
    heap.push(qe);
    delete qe;
    while (heap.get_size() > 0) {
        qe = heap.pop();
        uint u = *(qe->data);
        //W cdist = qe->value;
        chosen[u] = true;

        uint *neighbours, n_neighs;
        g.get_neighbours(u, &neighbours, &n_neighs);
        if (n_neighs > 0) {
            for (uint i = 0; i < n_neighs; i++) {
                uint v = neighbours[i];
                if (chosen[v])
                    continue;
                W rho_uv = g.get_edge_value(u, v);

                if (dists[v] <= rho_uv)
                    continue;
                dists[v] = rho_uv;
                preds[v] = u;
                qe->data = &nodes[v];
                qe->value = dists[v];
                heap.push(qe);
            }
        }
        delete qe;
    }
    results.success = true;
    for (uint k = 0; k < results.n_nodes; k++) {
        uint ind = results.nodes[k];
        if (dists[ind] >= (W) infinity) {
            results.success = false;
            break;
        }
        results.preds[k] = preds[ind];
    }

    delete[] nodes;
    delete[] preds;
    delete[] dists;
    delete[] chosen;
}

template <class W>
void MinSpanningTree<W>::prim(
    seed_t& seed, const Graph<W>& g, Graph<W>& h, bool& success
)
{
    PrimResults<W> results;
    prim_procedure(seed, g, h, results);
    success = results.success;
    if (!success)
        return;
    results.extract_mst(g, h);
}


template <class W>
void MinSpanningTree<W>::kruskal(
    seed_t& seed, const Graph<W>& g, Graph<W>& h, bool& success
)
{
    if (g.is_graph_directed()) {
        success = false;
        return;
    }
    uint n_nodes = g.get_order();
    uint **edges = 0;
    W *weights = 0;
    uint n_edges;
    g.get_edges(edges, weights, n_edges);
    uint *indices = new uint[n_edges];
    for (uint i = 0; i < n_edges; i++)
        indices[i] = i;
    quicksort(seed, weights, indices, 0, n_edges-1);
    //for (uint i = 0; i < n_edges; i++)
    //    printf("%i -> ", weights[indices[i]]);
    //printf("\n");

    uint *preds = new uint[n_nodes];
    uint *ranks = new uint[n_nodes];
    for (uint i = 0; i < n_nodes; i++) {
        preds[i] = i;
        ranks[i] = 0;
    }
    uint i_edge = 0;
    uint i_ind = 0;
    while (i_edge < n_nodes-1) {
        uint u = edges[indices[i_ind]][0];
        uint v = edges[indices[i_ind]][1];
        W w = weights[indices[i_ind]];
        i_ind += 1;
        uint pu = uf_find(preds, u, n_nodes);
        uint pv = uf_find(preds, v, n_nodes);
        if (pu == pv)
            continue;
        i_edge += 1;
        h.add_edge(u, v, w);
        uf_union(preds, ranks, u, v, n_nodes);
    }

    delete[] weights;
    delete[] indices;
    delete_2d_array(edges, n_edges);
    success = true;
}