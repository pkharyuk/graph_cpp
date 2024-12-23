// #include "graph.hpp" [not needed after moving to *.tpp]

// few helper functions

template <class W>
void Graph<W>::check_input_weight(W weight) const
{
    if (weight != 0)
        return;
    fprintf(stderr, "Zero weight is not allowed.\n");
    exit(EXIT_FAILURE);
}

template <class W>
void Graph<W>::check_input_node(uint u) const
{
    if (u < this->n_vertices)
        return;
    fprintf(stderr, "Node value is not allowed.\n");
    exit(EXIT_FAILURE);
}

template <class W>
void Graph<W>::ind_to_edge(
    uint ind,
    uint& i,
    uint& j
) const
{
    if (this->is_directed) {
        fprintf(
            stderr,
            "Graph is directed. Could not extract (i, j) (not implemented)\n"
        );
        exit(EXIT_FAILURE);
    }
    i = 0;
    j = 1;
    uint n_vertices = this->n_vertices - 1;
    while (ind >= n_vertices) {
        ind -= n_vertices;
        n_vertices -= 1;
        i += 1;
        j += 1;
    }
    j += ind;
}

// main methods

template <class W>
W Graph<W>::get_edge_value(uint u, uint v) const
{
    if (!this->is_edge(u, v))
        return 0;
    return this->adjacency[u][v];
}
template <class W>
W Graph<W>::get_edge_value(uint ind) const
{
    uint u = 0, v = 0;
    this->ind_to_edge(ind, u, v);
    return this->get_edge_value(u, v);
}

template <class W>
void Graph<W>::set_edge_value(uint u, uint v, W weight, bool check_exists)
{
    const char *func_name = "Graph::set_edge_value";
    if (check_exists && !this->is_edge(u, v)) {
        fprintf(
            stderr, "%s : edge does not exist", func_name
        );
        exit(EXIT_FAILURE);
    }
    this->check_input_weight(weight);
    this->adjacency[u][v] = weight;
    if (!this->is_directed)
        this->adjacency[v][u] = weight;
}

template <class W>
void Graph<W>::set_edge_value(uint ind, W weight, bool check_exists)
{
    uint u = 0, v = 0;
    this->ind_to_edge(ind, u, v);
    return this->set_edge_value(u, v, weight, check_exists);
}

template <class W>
void Graph<W>::init(uint n_vertices, bool is_directed)
{
    this->n_vertices = n_vertices;
    this->is_directed = is_directed;
    this->adjacency = 0;
    if (n_vertices > 0) {
        allocate_2d_array(this->adjacency, n_vertices);
    }
}

template <class W>
Graph<W>::Graph(uint n_vertices, bool is_directed)
{
    this->init(n_vertices, is_directed);
}

template <class W>
Graph<W>::Graph(const Graph& g)
{
    this->init(g.n_vertices, g.is_directed);
    if (this->n_vertices > 0)
        copy_2d_array(this->adjacency, g.adjacency, this->n_vertices);
}

template <class W>
Graph<W>::~Graph()
{
    if (this->adjacency) // nullptr
        return;
    delete_2d_array<W>(this->adjacency, this->n_vertices);
}

template <class W>
void Graph<W>::add_edge(uint u, uint v, W weight)
{
    const char *func_name = "Graph::add_edge";
    if (this->is_edge(u, v)) {
        uint c_weight = this->get_edge_value(u, v);
        if (c_weight == weight)
            return;
        fprintf(
            stderr, "%s : edge exists", func_name
        );
        exit(EXIT_FAILURE);
    }
    this->set_edge_value(u, v, weight, false);
    this->n_edges += 1;
}

template <class W>
void Graph<W>::add_edge(uint ind, W weight)
{
    uint u = 0, v = 0;
    this->ind_to_edge(ind, u, v);
    return this->add_edge(u, v, weight);
}

template <class W>
void Graph<W>::remove_edge(uint u, uint v)
{
    const char *func_name = "Graph::remove_edge";
    if (!this->is_edge(u, v)) {
        fprintf(
            stderr, "%s : edge does not exist", func_name
        );
        exit(EXIT_FAILURE);
    }
    this->set_edge_value(u, v, 0, false);
    this->n_edges -= 1;
}

template <class W>
void Graph<W>::remove_edge(uint ind)
{
    uint u = 0, v = 0;
    this->ind_to_edge(ind, u, v);
    return this->remove_edge(u, v);
}

template <class W>
bool Graph<W>::is_edge(uint u, uint v) const
{
    //const char *func_name = "Graph::is_edge";
    this->check_input_node(u);
    this->check_input_node(v);
    return this->adjacency[u][v] != 0;
}

template <class W>
bool Graph<W>::is_edge(uint ind) const
{
    uint u = 0, v = 0;
    this->ind_to_edge(ind, u, v);
    return this->is_edge(u, v);
}

template <class W>
void Graph<W>::get_neighbours(uint u, uint **neighbours, uint *n) const
{
    //const char *func_name = "Graph::get_neighbours";
    this->check_input_node(u);
    uint *tmp = new uint[this->n_vertices];
    *n = 0;
    for (uint v = 0; v < this->n_vertices; v++) {
        if (this->adjacency[u][v] == 0)
            continue;
        tmp[*n] = v;
        *n += 1;
    }
    *neighbours = new uint[*n];
    for (uint i = 0; i < *n; i++)
        (*neighbours)[i] = tmp[i];
    delete[] tmp;
}

template <class W>
Graph<W>::Graph(const char *fpath, bool is_directed)
{
    fprintf(
        stderr,
        "Filereader is not implemented for any weights but uint.\n"
    );
    exit(EXIT_FAILURE);
}

// partial template
template <>
Graph<uint>::Graph(const char *fpath, bool is_directed)
{
    TextFileReader reader(fpath);
    char *line = reader.read_line();
    uint n_vertices = safe_atou(line);
    delete[] line;

    this->init(n_vertices, is_directed);

    uint line_number = 1;
    while ((line = reader.read_line()) != 0) {
        uint *a = 0, n;
        string_to_uint_vec(line, &a, &n);
        switch (n) {
            case two:
                this->add_edge(a[0], a[1], 1);
                break;
            case three:
                this->add_edge(a[0], a[1], (uint64) a[2]);
                break;
            default:
                fprintf(
                    stderr,
                    "%s , line %u: expected %u or %u numbers per line\n",
                    fpath, line_number+1, two, three
                );
                exit(EXIT_FAILURE);
        }
        if (line)
            delete[] line;
        if (a)
            delete[] a;
        line_number += 1;
    }
}

template <class W>
void Graph<W>::get_vertices(uint **vertices, uint *n) const
{
    *n = 0;
    *vertices = new uint[this->n_vertices];
    for (uint i = 0; i < this->n_vertices; i++) {
        for (uint j = this->n_vertices-1; j >= 0; j--) {
            if (i == j)
                continue;
            bool is_ij_edge = this->is_edge(i, j);
            if (is_ij_edge) {
                (*vertices)[*n] = i;
                *n += 1;
                break;
            }
            if (j < i && !this->is_directed)
                break;
        }
    }
    if (*n == this->n_vertices)
        return;
    resize_1d_array<uint>(vertices, this->n_vertices, *n);
}

template <class W>
void Graph<W>::get_edges(uint **& edges, W *& weights, uint& n) const
{
    uint ind = 0;
    uint n_vertices = this->n_vertices;
    n = this->get_size();
    allocate_2d_array<uint>(edges, n, 2, false);
    weights = new W[n];
    bool is_this_directed = this->is_graph_directed();
    for (uint i = 0; i < n_vertices; i++) {
		uint j_start = 0;
		if (!is_this_directed)
			j_start = i+1;
		for (uint j = j_start; j < n_vertices; j++) {
			if (!this->is_edge(i, j))
				continue;
            edges[ind][0] = i;
            edges[ind][1] = j;
            W w = this->get_edge_value(i, j);
            weights[ind] = w;
            ind += 1;
        }
    }
}

// helper function that prints the adjacency matrix
// (its upper part, if we work with undirected graph)
template <class W>
void Graph<W>::print_adjacency(
    const char *weight_format_str,
    uint n_spaces
) const
{
    char *spaces_str = new char[n_spaces+1];
    for (uint i = 0; i < n_spaces; i++)
        spaces_str[i] = ' ';
    spaces_str[n_spaces] = '\0';
    const uint n = this->get_order();
    printf("Number of nodes: %u\n", n);
	for (uint i = 0; i < n; i++) {
		for (uint j = 0; j < n; j++) {
			if (j <= i && !this->is_graph_directed()) {
				printf(spaces_str);
                continue;
            }
            printf(weight_format_str, this->get_edge_value(i, j));
            printf(" ");
        }
        printf("\n");
	}
    delete[] spaces_str;
}


// helper function that prints the adjacency matrix
// (its upper part, as we work with undirected graph)
template <class W>
void Graph<W>::print_edges(const char *weight_format_str) const
{
	W total_cost = 0;
	uint n = this->get_order();
    bool is_it_directed = this->is_graph_directed();
	for (uint i = 0; i < n; i++) {
		uint j_start = 0;
		if (!is_it_directed)
			j_start = i+1;
		for (uint j = j_start; j < n; j++) {
			if (!this->is_edge(i, j))
				continue;
			W w = this->get_edge_value(i, j);
			printf("Edge (%2u, %2u): weight=", i, j);
            printf(weight_format_str, w);
            printf("\n");
            total_cost += w;
        }
	}
	printf("Total cost: ");
    printf(weight_format_str, total_cost);
    printf("\n");
}

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



// Author: Pavel Kharyuk, 2024