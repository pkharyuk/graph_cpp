#include "player.hpp"

// class UserPlayer;

bool UserPlayer::move(PlayBoard& board, int i, int j)
{
    return board.player_turn(
        i, j, this->get_color_code(), this->get_edge_value()
    );
}

// class BotPlayer

static inline void compute_shortest_path(
    ShortestPathResults<int>*& shortest_path_results,
    const Graph<int>* const& cell_graph,
    const int& bot_code, const uint& k, const uint& n
)
{
    uint src_node = BorderCellCollections::get_src_node_value(
        bot_code, k, n
    );
    shortest_path_results = ShortestPath<int>::find_shortest_path(
        *cell_graph, src_node
    );
}

void BotPlayer::extract_bot_shortest_path_solution(
    const PlayBoard& board, uint*& path, uint& path_len, int& path_cost
)
{
    ShortestPathResults<int> *shortest_path_results;
    path = 0;
    path_len = 0;
    path_cost = infinity;
    uint n = board.get_size();
    int bot_color = this->get_color_code();

    for (uint k = 1; k <= n; k++) {
        compute_shortest_path(
            shortest_path_results, board.get_cell_graph(), bot_color, k, n
        );
        for (uint l = 1; l <= n; l++) {
            uint dst_node = BorderCellCollections::get_dst_node_value(
                bot_color, l, n
            );
            if (
                (dynamic_cast<SingleSourceResults<int>*>(
                    shortest_path_results
                )->dists[dst_node] > infinity)
                || (dynamic_cast<SingleSourceResults<int>*>(
                    shortest_path_results
                )->dists[dst_node] >= path_cost)
            )
                continue;
            path_cost = dynamic_cast<SingleSourceResults<int>*>(
                shortest_path_results
            )->dists[dst_node];
            shortest_path_results->extract_shortest_path(dst_node);
            if (path)
                delete[] path;
            path = shortest_path_results->path;
            shortest_path_results->path = 0;
            path_len = shortest_path_results->path_len;
        }
    }
}

// true = successful move; false = end of game
void BotPlayer::extract_position_from_path(
    PlayBoard& board, uint* const& path, const uint& path_len,
    uint& i, uint& j
)
{
    uint n = board.get_size();
    bool success = false;
    int bot_color = this->get_color_code();
    for (uint k = 0; k < path_len-1; k++) {
        BorderCellCollections::convert_ind_to_ij(path[k], i, j, n, n);
        if (board.get_node_color(i, j) == bot_color)
            continue;
        if (k == path_len-2) // exclude src/dst nodes
            return;
        break;
    }
}

bool BotPlayer::move(
    PlayBoard& board, int i, int j
)
{
    uint *path = 0, path_len, i1, j1;
    int path_cost;
    uint n = board.get_size();
    int strategy = this->get_strategy();
    if (
        (i >= 0 && j >= 0)
        && BorderCellCollections::is_turn_position_valid(i, j, n, n)
    )
        return board.player_turn(
            i, j, this->get_color_code(), this->get_edge_value()
        );
    if (strategy == BotStrategy::random_strategy)
        strategy = random_integer_draw(
            this->seed, 0, BotStrategy::n_strategies-1
        );
    switch (strategy) {
        case BotStrategy::monte_carlo_lookahead:
            this->extract_monte_carlo_move(board, i1, j1, this->maxitnum);
            break;
        case BotStrategy::shortest_path_completion:
            this->extract_bot_shortest_path_solution(
                board, path, path_len, path_cost
            );
            if (path_cost < infinity)
                this->extract_position_from_path(
                    board, path, path_len, i1, j1
                );
            if (path)
                delete[] path;
            break;
        default:
            fprintf(stderr, "Invalid strategy for bot player.\n");
            exit(EXIT_FAILURE);
    };
    return board.player_turn(
        i1, j1, this->get_color_code(), this->get_edge_value()
    );
}

static inline void monte_carlo_iteration_fill_gaps(
    seed_t& seed, const PlayBoard& board, int **& ncolors,
    uint**& cnt, uint *& nodes, int color_code, uint n
)
{
    uint N = (n+2)*(n+2);
    for (uint i = 0; i < N; i++)
        nodes[i] = i;
    shuffle<uint>(seed, nodes, N);
    int current_color = color_code;
    for (uint k = 0; k < N; k++) {
        uint i, j;
        BorderCellCollections::convert_ind_to_ij(nodes[k], i, j, n, n);
        if (BorderCellCollections::is_corner(i, j, n, n))
            continue;
        int ij_color = board.get_node_color(i, j);
        if (ij_color == NodeColors::blank) {
            ncolors[i][j] = current_color;
            current_color = NodeColorsConverter::get_opponent_color(
                current_color
            );
        } else {
            ncolors[i][j] = ij_color;
        }
    }
}

static inline void monte_carlo_iteration(
    seed_t& seed, PlayBoard& board, int **& ncolors,
    uint**& cnt, uint *& nodes, int color_code, uint n
)
{
    uint N = (n+2)*(n+2);
    monte_carlo_iteration_fill_gaps(
        seed, board, ncolors, cnt, nodes, color_code, n
    );
    int winner = board.lookup_winner(ncolors);
    //printf("%s\n", NodeColorsConverter::get_color_name(winner));
    if (winner != color_code)
        return;
    for (uint k = 0; k < N; k++) {
        uint i, j;
        BorderCellCollections::convert_ind_to_ij(k, i, j, n, n);
        if (BorderCellCollections::is_corner(i, j, n, n))
            continue;
        int ij_color = board.get_node_color(i, j);
        if (ij_color == NodeColors::blank && ncolors[i][j] == color_code) {
            cnt[i][j] += 1;
        }
    }
}

static inline void find_greedy_move(
    uint** const& cnt, uint& u, uint& v, uint n
)
{
    uint max_val = 0;
    u = 0;
    v = 0;
    for (uint i = 0; i <= n+1; i++) {
        for (uint j = 0; j <= n+1; j++) {
            if (max_val >= cnt[i][j])
                continue;
            u = i;
            v = j;
            max_val = cnt[i][j];
        }
    }
}

static inline void initialize_monte_carlo_arrays(
    int **& ncolors, uint **& cnt, uint *& nodes, uint n
)
{
    uint N = (n+2)*(n+2); // number of vertices
    ncolors = new int*[n+2];
    cnt = new uint*[n+2];
    for (uint i = 0; i <= n+1; i++) {
        ncolors[i] = new int[n+2];
        cnt[i] = new uint[n+2];
        for (uint j = 0; j <= n+1; j++)
            cnt[i][j] = 0;
    }
    nodes = new uint[N];
}

static inline void remove_monte_carlo_arrays(
    int **& ncolors, uint **& cnt, uint *& nodes, uint n
)
{
    for (uint i = 0; i <= n+1; i++) {
        delete[] ncolors[i];
        delete[] cnt[i];
    }
    delete[] ncolors;
    delete[] cnt;
    delete[] nodes;
}

void BotPlayer::extract_monte_carlo_move(
    PlayBoard& board, uint& u, uint& v, uint n_simulations
)
{
    uint n = board.get_size();
    uint N = (n+2)*(n+2);
    int **ncolors = 0;
    uint **cnt = 0;
    uint *nodes = 0;
    initialize_monte_carlo_arrays(ncolors, cnt, nodes, n);
    for (uint itnum = 0; itnum < n_simulations; itnum++) {
        monte_carlo_iteration(
            this->seed, board, ncolors, cnt, nodes,
            this->get_color_code(), n
        );
    }
    find_greedy_move(cnt, u, v, n);
    //printf("%u %u %d\n", u, v, max_val);
    remove_monte_carlo_arrays(ncolors, cnt, nodes, n);
}