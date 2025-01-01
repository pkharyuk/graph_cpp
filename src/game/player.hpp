#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "game_types.hpp"
//#include "custom_types.hpp"
#include "custom_random.hpp"
#include "board.hpp"

class Player {
    int color_code;
    int edge_value;

public:
    Player(int color_code_in, int edge_value_in)
        : color_code(color_code_in)
        , edge_value(edge_value_in)
        {}
    virtual ~Player()
        {}

    int get_color_code() const { return color_code; }
    int get_edge_value() const { return edge_value; }
    virtual bool move(PlayBoard& board, int i, int j) = 0;
};

class UserPlayer : public Player {
public:
    UserPlayer(int color_code_in, int edge_value_in)
        : Player(color_code_in, edge_value_in)
        {};
    bool move(
        PlayBoard &board, int i, int j
    );
};

class BotPlayer : public Player {
    seed_t seed;
    uint maxitnum;
    int strategy;

public:
    BotPlayer(
        int color_code_in, int edge_value_in,
        seed_t seed_in = 0, uint maxitnum_in = 1000,
        int strategy_in = BotStrategy::monte_carlo_lookahead
    )
        : Player(color_code_in, edge_value_in)
        , seed(seed_in)
        , maxitnum(maxitnum_in)
        , strategy(strategy_in)
        {};
    bool move(
        PlayBoard& board, int i = -1, int j = -1
    );
    int get_strategy() const { return strategy; }

private:
    void extract_bot_shortest_path_solution(
        const PlayBoard& board,
        uint*& path, uint& path_len, int& path_cost
    );
    void extract_position_from_path(
        PlayBoard& board, uint* const& path, const uint& path_len,
        uint& i, uint& j
    );
    void extract_monte_carlo_move(
        PlayBoard& board, uint& i, uint& j,
        uint n_simulations
    );
};



#endif