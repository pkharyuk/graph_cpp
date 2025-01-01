#include "board.hpp"

static const int di[] = {-1, -1, 0, 0, 1, 1};
static const int dj[] = {0, 1, -1, 1, -1, 0};

static inline void init_node_color(
    int**& ncolor, const uint& i, const uint& j, const uint& m, const uint& n
)
{
    ncolor[i][j] = NodeColors::blank;
    if ((i == 0 || i == m+1) && (j > 0 && j <= n))
        ncolor[i][j] = NodeColors::red;
    if ((j == 0 || j == n+1) && (i > 0 && i <= m))
        ncolor[i][j] = NodeColors::blue;
}

void PlayBoard::add_neighbor(
    const uint& u, const uint& neigh_i, const uint& neigh_j,
    const int& fill_value
)
{
    uint n = this->get_size();
    if (BorderCellCollections::is_corner(neigh_i, neigh_j, n, n))
        return;
    uint v = BorderCellCollections::convert_ij_to_ind(
        neigh_i, neigh_j, n, n
    );
    this->cell_graph->add_edge(u, v, fill_value);
}


void PlayBoard::remove_neighbor(
    const uint& u, const uint& neigh_i, const uint& neigh_j
)
{
    uint n = this->get_size();
    if (BorderCellCollections::is_corner(neigh_i, neigh_j, n, n))
        return;
    uint v = BorderCellCollections::convert_ij_to_ind(
        neigh_i, neigh_j, n, n
    );
    if (this->cell_graph->is_edge(u, v))
        this->cell_graph->remove_edge(u, v);
}


void PlayBoard::set_neighbor(
    const uint& u, const uint& neigh_i, const uint& neigh_j,
    const int& fill_value
)
{
    uint n = this->get_size();
    if (BorderCellCollections::is_corner(neigh_i, neigh_j, n, n))
        return;
    uint v = BorderCellCollections::convert_ij_to_ind(
        neigh_i, neigh_j, n, n
    );
    if (this->cell_graph->is_edge(u, v))
        this->cell_graph->set_edge_value(u, v, fill_value, false);
}


// unsafe
int PlayBoard::get_node_color(uint i, uint j) const
{
    uint n = this->get_size();
    if (
        (i < 0 || i > n+1 || j < 0 || j > n+1)
        || (BorderCellCollections::is_corner(i, j, n, n))
    )
        return NodeColors::not_a_color;
    return this->node_colors[i][j];
}

static inline void print_adjacency(const Graph<int>& g)
{
	uint n = g.get_order();
	printf("%u\n", n);
	for (uint i = 0; i < n; i++) {
		for (uint j = 0; j < n; j++) {
			printf("%2d ", g.get_edge_value(i, j));
		}
		printf("\n");
	}
}

static inline void draw_first_line(
    const uint& i, const uint& n,
    uint& j_start, uint& j_end,
    int** const& ncolors
)
{
    char c;
    uint j;
    for (j = 0; j < 2*i; j++)
        printf(" ");
    if (i == 0 || i == n+1) {
        printf("    ");
        j_start = 1;
        j_end = n;
    }
    for (j = j_start; j < j_end; j++) {
        c = NodeColorsConverter::decode(ncolors[i][j]);
        printf("%c - ", c);
    }
    c = NodeColorsConverter::decode(ncolors[i][j]);
    printf("%c\n", c);
}

static inline void draw_second_line(
    uint& i, uint& n, uint& j_start, uint& j_end
)
{
    uint j;
    if (i == n) {
        j_start = 1;
        j_end = n;
        printf("    ");
    }
    for (j = 0; j < 2*i+1; j++)
        printf(" ");
    if (i == 0)
        printf("    ");
    for (j = j_start; j < j_end; j++)
        printf("\\ / ");
    if (i == n)
        printf("\\  \n");
    else
        printf("\\\n");
}


void PlayBoard::draw() const
{
    uint i, j, j_start, j_end;
    char c;
    uint n = this->size;
    for (i = 0; i <= n+1; i++) {
        j_start = 0;
        j_end = n+1;

        draw_first_line(
            i, n, j_start, j_end, this->node_colors
        );
        if (i == n+1)
            break;
        draw_second_line(i, n, j_start, j_end);
    }
    //print_adjacency(*this->cell_graph);
}


bool PlayBoard::player_turn(uint i, uint j, int player_color, int edge_value)
{
    uint n = this->size;
    bool valid = BorderCellCollections::is_turn_position_valid(i, j, n, n);
    if (!valid)
        return false;
    if (this->node_colors[i][j] != NodeColors::blank)
        return false;
    this->node_colors[i][j] = player_color;
    uint u = BorderCellCollections::convert_ij_to_ind(i, j, n, n);
    uint n_neighbors = sizeof(di) / sizeof(*di);
    int opponent_color = NodeColorsConverter::get_opponent_color(player_color);
    for (uint k = 0; k < n_neighbors; k++) {
        uint k_i = i + di[k];
        uint k_j = j + dj[k];
        if (this->get_node_color(k_i, k_j) == NodeColors::blank)
            set_neighbor(
                u, k_i, k_j, edge_value
            );
        else if (this->get_node_color(k_i, k_j) == opponent_color)
            remove_neighbor(u, k_i, k_j);
    }
    return true;
}



// was shocked when have read comments like "use containers/vectors"
// can only advice them to learn how to use valgrind
PlayBoard::~PlayBoard()
{
    if (this->cell_graph)
        delete this->cell_graph;

    for (uint i = 0; i <= this->size+1; i++) {
        if (this->node_colors[i] != 0)
            delete[] this->node_colors[i];
    }
    delete[] this->node_colors;
}

void PlayBoard::init()
{
    if (this->size % 2 == 0) {
        fprintf(stderr, "Invalid board size: must be odd.\n");
        exit(EXIT_FAILURE);
    }
    //this->bot_code = NodeColorsConverter::get_opponent_color(
    //    this->player_code
    //);
    uint n = this->size;
    uint n_nodes = (n+2)*(n+2);
    this->cell_graph = new Graph<int>(n_nodes, false);
    this->node_colors = new int*[n+2];
    uint n_neighbors = sizeof(di) / sizeof(*di);
    for (uint i = 0; i <= n+1; i++) {
        this->node_colors[i] = new int[n+2];
        for (uint j = 0; j <= n+1; j++) {
            init_node_color(this->node_colors, i, j, n, n);
            if (BorderCellCollections::is_border(i, j, n, n))
                continue;
            uint u = BorderCellCollections::convert_ij_to_ind(i, j, n, n);
            for (uint k = 0; k < n_neighbors; k++) {
                uint k_i = i + di[k];
                uint k_j = j + dj[k];
                this->add_neighbor(u, k_i, k_j, init_fill_value);
            }
        }
    }
}


static inline void recursive_bfs_lookup(
    int ** const& ncolors,
    uint& node,
    int value,
    bool*& visited,
    uint& n
)
{
    if (visited[node])
        return;
    visited[node] = true;
    uint i, j, v;
    BorderCellCollections::convert_ind_to_ij(node, i, j, n, n);
    uint n_neighbors = sizeof(di) / sizeof(*di);
    for (uint k = 0; k < n_neighbors; k++) {
        int k_i = i + di[k];
        int k_j = j + dj[k];
        if (k_i > n+1 || k_j > n+1 || k_i < 0 || k_j < 0)
            continue;
        if (ncolors[k_i][k_j] != value)
            continue;
        v = BorderCellCollections::convert_ij_to_ind(k_i, k_j, n, n);
        recursive_bfs_lookup(ncolors, v, value, visited, n);
    }
}

static inline bool probe_final_destination(
    bool*& visited, uint n, bool probe_bottom
)
{
    uint k, node;
    for (k = 0; k <= n+1; k++) {
        if (probe_bottom)
            node = BorderCellCollections::convert_ij_to_ind(n+1, k, n, n);
        else // probe_right
            node = BorderCellCollections::convert_ij_to_ind(k, n+1, n, n);
        if (visited[node])
            return true;
    }
    return false;
}

int PlayBoard::lookup_winner(int **ncolors)
{
    bool dry_run = true;
    if (!ncolors) {
        ncolors = this->node_colors;
        dry_run = false;
    }

    uint N = this->cell_graph->get_order();
    uint n = this->get_size();
    bool *visited = new bool[N];
    for (uint i = 0; i < N; i++)
        visited[i] = false;
    // try to find path from upper part to bottom
    for (uint j = 0; j <= n+1; j++) {
        uint node = BorderCellCollections::convert_ij_to_ind(0, j, n, n);
        recursive_bfs_lookup(
            ncolors, node, NodeColors::red, visited, n
        );
    }
    bool is_red_destination = probe_final_destination(visited, n, true);
    for (uint i = 0; i < N; i++)
        visited[i] = false;
    // try to find path from left part to right
    for (uint i = 0; i <= n+1; i++) {
        uint node = BorderCellCollections::convert_ij_to_ind(i, 0, n, n);
        recursive_bfs_lookup(
            ncolors, node, NodeColors::blue, visited, n
        );
    }
    bool is_blue_destination = probe_final_destination(visited, n, false);
    delete[] visited;
    if (!is_red_destination && !is_blue_destination)
        return NodeColors::blank;
    if (!dry_run)
        this->is_winner = true;
    if (is_red_destination) {
        if (!dry_run)
            this->winner = NodeColors::red;
        return NodeColors::red;
    }
    if (!dry_run)
        this->winner = NodeColors::blue;
    return NodeColors::blue;
}