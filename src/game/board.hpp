#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include "game_types.hpp"
#include "custom_types.hpp"
//#include "graph.hpp"
#include "shortest_path.hpp"

/*
. - . - . - . - .
 \ / \ / \ / \ / \
  . - . - . - . - .
   \ / \ / \ / \ / \
    . - . - . - . - .
     \ / \ / \ / \ / \
      . - . - . - . - .
       \ / \ / \ / \ / \
        . - . - . - . - .
*/

// helper structure encapsulating several common functions
// related to the board positioning
struct BorderCellCollections {
    static int convert_ij_to_ind(
        const uint& i, const uint &j, const uint& m, const uint& n
    );
    static void convert_ind_to_ij(
        const uint& ind, uint& i, uint& j, const uint& m, const uint& n
    );
    static bool is_corner(
        const uint& i, const uint& j, const uint& m, const uint& n
    );
    static bool is_border(
        const uint& i, const uint& j, const uint& m, const uint& n
    );
    static bool is_turn_position_valid(
        const uint& i, const uint& j, const uint& m, const uint& n
    );
    static uint get_src_node_value(
        const int& bot_code, const uint& k, const uint& n
    );
    static uint get_dst_node_value(
        const int& bot_code, const uint& k, const uint& n
    );

};

// friend class header (fwd declaration)

// playing board
class PlayBoard {
    uint size;
    //Player *red_player;
    //Player *blue_player;
    Graph<int> *cell_graph;
    int **node_colors;
    int winner;
    bool is_winner;
    uint current_turn;

    friend class BotPlayer;

public:
    PlayBoard(uint size_in)//, int player_code_in = NodeColors::blank)
        : size(size_in)
        //, red_player(0)
        //, blue_player(0)
        , node_colors(0)
        , winner(0)
        , is_winner(false)
        , current_turn(0)
        { this->init(); }
        //{ this->init(player_code_in); }
    
    ~PlayBoard();

    uint get_size() const { return size; }
    int get_node_color(uint i, uint j) const;
    uint get_current_turn_num() const { return current_turn; }
    int get_winner() const { return winner; }
    bool is_game_active() const { return !(is_winner); }
    void increment_turn_counter() { current_turn += 1; }

    void draw() const;
    bool player_turn(uint i, uint j, int player_color, int edge_value);
    int lookup_winner(int **ncolors = 0);

private:
    void init(); //int player_code_in);
    void fill_node_color(uint i, uint j, int color)
        { this->node_colors[i][j] = color; }
    int **get_node_colors() const { return node_colors; }
    Graph<int> *get_cell_graph() const { return cell_graph; }
    //IntGraph *get_cell_graph() const { return cell_graph; }


    void add_neighbor(
        const uint& u, const uint& neigh_i, const uint& neigh_j,
        const int& fill_value
    );
    void remove_neighbor(
        const uint& u, const uint& neigh_i, const uint& neigh_j
    );
    void set_neighbor(
        const uint& u, const uint& neigh_i, const uint& neigh_j,
        const int& fill_value
    );

};

// inline functions

inline int BorderCellCollections::convert_ij_to_ind(
    const uint& i, const uint& j, const uint& m, const uint& n
)
{
    return i*(n+2) + j;
}

inline void BorderCellCollections::convert_ind_to_ij(
    const uint& ind, uint& i, uint& j, const uint& m, const uint& n
)
{
    i = (ind) / (n+2);
    j = (ind) % (n+2);
}

inline bool BorderCellCollections::is_corner(
    const uint& i, const uint& j, const uint& m, const uint& n
)
{
    return (
        ((i == 0) && (j == 0))
        || ((i == 0) && (j == n+1))
        || ((i == m+1) && (j == 0))
        || ((i == m+1) && (j == n+1))
    );
}

inline bool BorderCellCollections::is_border(
    const uint& i, const uint& j, const uint& m, const uint& n
)
{
    return (
        (i == 0) || (i == m+1) || (j == 0) || (j == n+1)
    );
}

inline bool BorderCellCollections::is_turn_position_valid(
    const uint& i, const uint& j, const uint& m, const uint& n
)
{
    if (i > m || j > n)
        return false;
    if (BorderCellCollections::is_border(i, j, m, n))
        return false;
    return true;
}


inline uint BorderCellCollections::get_src_node_value(
    const int& bot_code, const uint& k, const uint& n
)
{
    if (bot_code == NodeColors::red)
        return BorderCellCollections::convert_ij_to_ind(0, k, n, n);
    return BorderCellCollections::convert_ij_to_ind(k, 0, n, n);
}

inline uint BorderCellCollections::get_dst_node_value(
    const int& bot_code, const uint& k, const uint& n
)
{
    if (bot_code == NodeColors::red)
        return BorderCellCollections::convert_ij_to_ind(n+1, k, n, n);
    return BorderCellCollections::convert_ij_to_ind(k, n+1, n, n);
}


#endif
// Author: Pavel Kharyuk, 2024