#include "sample_graph.hpp"

// custom graph sampler
Graph<uint>* sample_random_undirected_uint_graph(
    seed_t& seed,
    uint n_vertices,
    double density,
    uint min_distance,
    uint max_distance
)
{
    const char *func_name = "sample_random_undirected_graph";
    if (density < 0 || density > 1) {
        fprintf(
            stderr,
            "%s : density is out from [0, 1]",
            func_name
        );
        exit(EXIT_FAILURE);
    }
    Graph<uint> *g = new Graph<uint>(n_vertices, false);
    
    uint n_full_edges = n_vertices*(n_vertices - 1)/2;
    uint n_edges = (uint) (density*n_full_edges + 0.5);
    uint *weights = (uint *) vec_random_integer_draw(
        seed, n_edges, min_distance, max_distance
    );
    uint *indices = new uint[n_full_edges];
    for (uint k = 0; k < n_full_edges; k++)
        indices[k] = k;
    shuffle(seed, indices, n_full_edges, n_edges);
    for (uint k = 0; k < n_edges; k++) {
        //uint i, j;
        //ind_to_edge(n_vertices, indices[k], i, j);
        //printf("%u -> (%u, %u)\n", indices[k], i, j);
        //g->add_edge(i, j, weights[k]);
        g->add_edge(indices[k], weights[k]);
    }
    delete[] weights;
    delete[] indices;
    return g;
}