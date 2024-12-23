//#include "shortest_path.hpp"

///////////////////////////////////////////////////////////////////////////
// class ShortestPathResults;
///////////////////////////////////////////////////////////////////////////
template <class W>
void ShortestPathResults<W>::flush()
{
    this->path_len = 0;
    if (!this->path)
        return;
    delete[] this->path;
    this->path = 0;
}

template <class W>
void ShortestPathResults<W>::check_node(
    int input_node, const char *which_node
) const
{
    if (input_node < 0) {
        fprintf(
            stderr,
            "Please specify the %s node. Aborted.\n",
            which_node
        );
        exit(EXIT_FAILURE);
    }
    if ((const uint) input_node >= this->n_nodes) {
        fprintf(
            stderr,
            "Invalid node passed (%s). Aborted.\n",
            which_node
        );
        exit(EXIT_FAILURE);
    }
}

template <class W>
void ShortestPathResults<W>::check_src_dst_nodes(
    int input_src_node, int input_dst_node
) const
{
    this->check_node(input_src_node, "source");
    this->check_node(input_dst_node, "destination");
}

template <class W>
uint ShortestPathResults<W>::get_shortest_path_len(
    int src_node, int dst_node
) const
{
    if (this->src_node < 0 || this->src_node != src_node) {
        fprintf(stderr, "No precomputed results for the given src_node.\n");
        exit(EXIT_FAILURE);
    }
    if (this->dst_node < 0 || this->dst_node != dst_node) {
        fprintf(stderr, "No precomputed results for the given dst_node.\n");
        exit(EXIT_FAILURE);
    }
    return this->path_len;
}

template <class W>
void ShortestPathResults<W>::shortest_path_extractor(
    uint src_node, uint dst_node, uint *preds
)
{
    this->check_src_dst_nodes(src_node, dst_node);
    this->flush();
    this->src_node = src_node;
    this->dst_node = dst_node;

    uint u = dst_node;
    uint *tmp_path = new uint[this->n_nodes];
    while (u != src_node) {
        tmp_path[this->n_nodes - this->path_len - 1] = u;
        this->path_len += 1;
        u = preds[u];
    }
    tmp_path[this->n_nodes - this->path_len - 1] = u;
    if (this->path_len == this->n_nodes) {
        this->path = tmp_path;
        return;
    }
    uint offset = this->n_nodes - this->path_len - 1;
    this->path = new uint[this->path_len+1];
    for (uint i = 0; i <= this->path_len; i++)
        this->path[i] = tmp_path[i+offset];
    delete[] tmp_path;
}

///////////////////////////////////////////////////////////////////////////
// class SingleSourceResults : ShortestPathResults;
///////////////////////////////////////////////////////////////////////////

// frees occupied memory
template <class W>
void SingleSourceResults<W>::flush()
{
    if (this->preds) {
        delete[] this->preds;
        this->preds = 0;
    }
    if (this->dists) {
        delete[] this->dists;
        this->dists = 0;
    }
}

// modifies input_{src,dst}_node
template <class W>
void SingleSourceResults<W>::check_single_source_results_inputs(
    int& input_src_node, int& input_dst_node
) const
{
    if (
        this->src_node < 0
        || input_src_node != this->src_node
        //|| (input_src_node >= 0 && input_src_node != instance_src_node)
    ) {
        fprintf(
            stderr,
            "No precomputed results (check src_node)\n"
        );
        exit(EXIT_FAILURE);
    }
    if (this->dst_node != input_dst_node && !this->all_nodes_visited)
        fprintf(
            stderr,
            "Attempted to extract the path w/o visiting all reachable nodes\n"
        );
    if (input_src_node < 0)
        input_src_node = this->src_node;
    if (input_dst_node < 0)
        input_dst_node = this->dst_node;
    this->check_src_dst_nodes(input_src_node, input_dst_node);
}

// operates on precomputed predecessors list
// v = pred[u] means that there is an edge from u to v
// in the shortest path previously found
template <class W>
void SingleSourceResults<W>::extract_shortest_path(
    int src_node, int dst_node
)
{
    this->check_single_source_results_inputs(src_node, dst_node);
    this->base_flush();
    if (!this->is_node_reachable(dst_node)) {
        fprintf(stderr, "Could not find a path.\n");
        return;
    }
    this->shortest_path_extractor(src_node, dst_node, this->preds);
}

// returns the sum of edge values constituting the shortest path
template <class W>
W SingleSourceResults<W>::get_shortest_path_cost(
    int src_node, int dst_node
) const
{
    this->check_single_source_results_inputs(src_node, dst_node);
    return this->dists[dst_node];
}

template <class W>
bool SingleSourceResults<W>::is_node_reachable(
    uint dst_node, int src_node
) const
{
    // src_node check ?
    if (dst_node >= this->n_nodes) {
        fprintf(stderr, "Invalid node passed\n");
        exit(EXIT_FAILURE);
    }
    if (!this->dists)
        return false;
    return (this->dists[dst_node] < (W) infinity);
}

///////////////////////////////////////////////////////////////////////////
// class FloydWarshallResults : ShortestPathResults;
///////////////////////////////////////////////////////////////////////////

// frees occupied memory
template <class W>
void FloydWarshallResults<W>::flush()
{
    delete_2d_array<uint>(this->preds, this->n_nodes);
    this->preds = 0;
    delete_2d_array<uint>(this->dists, this->n_nodes);
    this->dists = 0;
}

template <class W>
void FloydWarshallResults<W>::check_floyd_warshall_results_inputs(
    int input_src_node, int input_dst_node
) const
{
    this->check_src_dst_nodes(input_src_node, input_dst_node);
}

// modifies this->{src,dst}_node
template <class W>
void FloydWarshallResults<W>::extract_shortest_path(
    int src_node, int dst_node
)
{
    this->check_floyd_warshall_results_inputs(src_node, dst_node);
    //this->src_node = input_src_node;
    //this->dst_node = input_dst_node;
    this->base_flush();
    if (this->dists[src_node][dst_node] >= (W) infinity) {
        fprintf(stderr, "Could not find a path.\n");
        return;
    }
    //for (uint i = 0; i < this->n_nodes; i++)
    //    printf("%i -> %i\n", this->preds[src_node][i], i);
    this->shortest_path_extractor(
        src_node, dst_node, this->preds[src_node]
    );
}

template <class W>
W FloydWarshallResults<W>::get_shortest_path_cost(
    int src_node, int dst_node
) const
{
    this->check_floyd_warshall_results_inputs(src_node, dst_node);
    return this->dists[src_node][dst_node];
}

template <class W>
bool FloydWarshallResults<W>::is_node_reachable(
    uint dst_node, int src_node
) const
{
    if (dst_node >= this->n_nodes) {
        fprintf(stderr, "Invalid node passed\n");
        exit(EXIT_FAILURE);
    }
    if (!this->dists)
        return false;
    return (this->dists[src_node][dst_node] < (W) infinity);
}


///////////////////////////////////////////////////////////////////////////
// class ShortestPath;
///////////////////////////////////////////////////////////////////////////

// dijkstra algorithm + path extraction
template <class W>
ShortestPathResults<W>* ShortestPath<W>::find_shortest_path(
    const Graph<W>& g,
    uint src_node,
    int dst_node,
    enum ShortestPathAlgorithm code
)
{
    bool is_dst_node = dst_node >= 0;
    ShortestPathResults<W> *results;
    switch (code) {
        case ShortestPathAlgorithm::dijkstra:
            SingleSourceResults<W> *dijkstra_results;
            ShortestPath<W>::dijkstra(
                g, src_node, dst_node,
                //&results,
                dijkstra_results,
                !is_dst_node
            );
            results = dijkstra_results;
            break;
        case ShortestPathAlgorithm::floyd_warshall:
            FloydWarshallResults<W> *fw_results;
            ShortestPath<W>::floyd_warshall(
                g,
                fw_results
            );
            fw_results->src_node = src_node;
            results = fw_results;
            break;
        case ShortestPathAlgorithm::bellman_ford:
            SingleSourceResults<W> *bf_results;
            ShortestPath<W>::bellman_ford(
                g, src_node, dst_node, bf_results
            );
            results = bf_results;
            break;
        default:
            fprintf(
                stderr,
                "Invalid shortest path algorithm code provided (%i),\n",
                code
            );
            exit(EXIT_FAILURE);
            break;
    }
    if (is_dst_node)
        results->extract_shortest_path();
    return results;
}

// note: preds and dists should be deleted in a caller function
template <class W>
void ShortestPath<W>::dijkstra(
    const Graph<W>& g,
    uint src_node,
    int input_dst_node,
    SingleSourceResults<W> *& results,
    bool visit_all
)
{
    uint dst_node = src_node;
    if (input_dst_node >= 0)
        dst_node = input_dst_node;

    results = new SingleSourceResults<W>(
        g.get_order(), src_node, dst_node
    );
    (results)->dists = new W[(results)->n_nodes];
    (results)->preds = new uint[(results)->n_nodes];
    uint *nodes = new uint[(results)->n_nodes];
    for (uint i = 0; i < (results)->n_nodes; i++) {
        (results)->dists[i] = infinity;
        (results)->preds[i] = i;
        nodes[i] = i;
    }
    (results)->dists[src_node] = 0;
    MinHeap<uint, W, uint> heap;
    QueueElement<uint, W> *qe = new QueueElement<uint, W>(
        &nodes[src_node], (results)->dists[src_node]
    );
    heap.push(qe);
    delete qe;
    while (heap.get_size() > 0) {
        qe = heap.pop();
        uint u = *(qe->data);
        W cdist = qe->value;

        if (u == dst_node && !visit_all)
            break;

        uint *neighbours, n_neighs;
        g.get_neighbours(u, &neighbours, &n_neighs);
        if (n_neighs > 0) {
            for (uint i = 0; i < n_neighs; i++) {
                uint v = neighbours[i];
                W rho_uv = g.get_edge_value(u, v);
                if ((results)->dists[v] <= cdist + rho_uv)
                    continue;
                (results)->dists[v] = cdist + rho_uv;
                (results)->preds[v] = u;
                qe->data = &nodes[v];
                qe->value = (results)->dists[v];
                heap.push(qe);
            }
        }
        delete qe;
    }
    delete[] nodes;
    (results)->all_nodes_visited = visit_all;
}

template <class W>
void ShortestPath<W>::floyd_warshall(
    const Graph<W>& g,
    FloydWarshallResults<W> *& results
)
{
    uint n_nodes = g.get_order();
    bool is_directed = g.is_graph_directed();
    results = new FloydWarshallResults<W>(n_nodes);
    allocate_2d_array<uint>((results)->preds, n_nodes, n_nodes, false);
    allocate_2d_array<W>((results)->dists, n_nodes, n_nodes, false);
    for (uint src_node = 0; src_node < n_nodes; src_node++) {
        for (uint dst_node = 0; dst_node < n_nodes; dst_node++) {
            (results)->preds[src_node][dst_node] = src_node;
            if (dst_node == src_node) {
                (results)->dists[src_node][dst_node] = 0;
                continue;
            }
            if (dst_node > src_node && !is_directed)
                break;
            if (!g.is_edge(src_node, dst_node))
                (results)->dists[src_node][dst_node] = infinity;
            else
                (results)->dists[src_node][dst_node] = g.get_edge_value(
                    src_node, dst_node
                );
            if (!is_directed) {
                (results)->dists[dst_node][src_node] = (
                    (results)->dists[src_node][dst_node]
                );
                (results)->preds[dst_node][src_node] = dst_node;
            }
        }
    }
    for (uint k = 0; k < n_nodes; k++) {
        for (uint src_node = 0; src_node < n_nodes; src_node++) {
            W dist_sk = (results)->dists[src_node][k];
            for (uint dst_node = 0; dst_node < n_nodes; dst_node++) {
                if (dst_node >= src_node && !is_directed)
                    break;
                W dist_kt = (results)->dists[k][dst_node];
                W dist_st = (results)->dists[src_node][dst_node];
                if (dist_st <= dist_sk + dist_kt)
                    continue;
                (results)->dists[src_node][dst_node] = dist_sk + dist_kt;
                (results)->preds[src_node][dst_node] = (
                    (results)->preds[k][dst_node]
                );
                if (is_directed)
                    continue;
                (results)->dists[dst_node][src_node] = dist_sk + dist_kt;
                (results)->preds[dst_node][src_node] = (
                    (results)->preds[k][src_node]
                );
            }
        }
    }
}


template <class W>
void ShortestPath<W>::bellman_ford(
    const Graph<W>& g,
    uint src_node,
    int input_dst_node,
    SingleSourceResults<W> *& results
)
{
    uint n_edges = 0;
    uint **edges = 0;
    W *weights = 0;
    g.get_edges(edges, weights, n_edges);
    bool is_directed = g.is_graph_directed();

    uint dst_node = src_node;
    if (input_dst_node >= 0)
        dst_node = input_dst_node;

    results = new SingleSourceResults<W>(
        g.get_order(), src_node, dst_node
    );
    results->dists = new W[results->n_nodes];
    results->preds = new uint[results->n_nodes];
    for (uint i = 0; i < results->n_nodes; i++) {
        results->dists[i] = infinity;
        results->preds[i] = i;
    }
    results->dists[src_node] = 0;

    for (uint i = 0; i < g.get_order(); i++) {
        for (uint j = 0; j < n_edges; j++) {
            uint u = edges[j][0];
            uint v = edges[j][1];
            W w = weights[j];
            bool updated = false;
            if (
                (results->is_node_reachable(u, src_node))
                && (results->dists[u] + w < results->dists[v])
            ) {
                updated = true;
                results->dists[v] = results->dists[u] + w;
                results->preds[v] = u;
            }
            if (
                !is_directed
                && (results->is_node_reachable(v, src_node))
                && (results->dists[v] + w < results->dists[u])
            ) {
                updated = true;
                results->dists[u] = results->dists[v] + w;
                results->preds[u] = v;
            }
            if (i == g.get_order()-1 && updated) {
                fprintf(
                    stderr,
                    "Negative cycle detected.\n"
                );
                delete[] results->dists;
                results->dists = 0;
                delete[] results->preds;
                results->preds = 0;
                return;
            }
        }
    }
    results->all_nodes_visited = true;
    delete_2d_array<uint>(edges, n_edges);
    delete[] weights;
}

template <class W>
double ShortestPath<W>::compute_average_shortest_path_length_for_node(
	const Graph<W>& g,
    uint src_node,
    ShortestPathResults<W> *shortest_path_results,
    enum ShortestPathAlgorithm code
)
{
    bool compute_spr = (shortest_path_results == 0);
    if (compute_spr) {
        shortest_path_results = ShortestPath::find_shortest_path(
			g, src_node, -1, code
		);
    }
    if (shortest_path_results->path != 0) {
        delete[] shortest_path_results->path;
        shortest_path_results->path = 0;
        shortest_path_results->path_len = 0;
    }
    uint n_success = 0;
    uint64 cum_length = 0;
	for (uint dst_node = 0; dst_node < g.get_order(); dst_node++) {
        if (
            (dst_node == src_node)
            || (!shortest_path_results->is_node_reachable(dst_node, src_node))
        )
            continue;
        shortest_path_results->extract_shortest_path(src_node, dst_node);
        for (uint k = 0; k < shortest_path_results->path_len; k++)
            printf("%u -> ", shortest_path_results->path[k]);
        printf("%u\n", shortest_path_results->path[shortest_path_results->path_len]);
        n_success += 1;
        cum_length += shortest_path_results->path_len;
        delete[] shortest_path_results->path;
        shortest_path_results->path = 0;
        shortest_path_results->path_len = 0;
    }
    if (compute_spr)
        delete shortest_path_results;
    double avg_shortest_path_len = cum_length;
	avg_shortest_path_len /= (double) n_success;
    return avg_shortest_path_len;
}

template <class W>
double ShortestPath<W>::compute_average_shortest_path_cost_for_node(
	const Graph<W>& g,
    uint src_node,
    ShortestPathResults<W> *shortest_path_results,
    enum ShortestPathAlgorithm code
)
{
    bool compute_spr = (shortest_path_results == 0);
    if (compute_spr)
        shortest_path_results = ShortestPath::find_shortest_path(
			g, src_node, -1, code
		);
    uint n_success = 0;
    W cum_weight = 0;
	for (uint dst_node = 0; dst_node < g.get_order(); dst_node++) {
        if (
            (dst_node == src_node)
            || (!shortest_path_results->is_node_reachable(dst_node, src_node))
        )
            continue;
        n_success += 1;
        cum_weight += shortest_path_results->get_shortest_path_cost(
            src_node, dst_node
        );
    }
    if (compute_spr)
        delete shortest_path_results;
    double avg_shortest_path_cost = cum_weight;
	avg_shortest_path_cost /= (double) n_success;
    return avg_shortest_path_cost;
}


// Author: Pavel Kharyuk, 2024
