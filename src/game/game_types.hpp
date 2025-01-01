#ifndef __GAME_TYPES_HPP__
#define __GAME_TYPES_HPP__

#include <stdio.h>
#include <stdlib.h>
#include "custom_types.hpp"
#include "graph.hpp"

// encodes edge of node chosen by player
const int player_chosed_fill_value = infinity;
// encodes edge of available node
const int init_fill_value = 50;
// encodes edge of node chosen by bot
const int bot_chosed_fill_value = 1;

// must be contiguous starting from 0
enum BotStrategy {
    monte_carlo_lookahead = 0,
    shortest_path_completion = 1,
    random_strategy,
    n_strategies
};

enum NodeColors {
    red = 1,
    blue = -1,
    blank = 0,
    not_a_color = 10
};

struct NodeColorsConverter {

    static enum NodeColors get_enum_color_from_int(int code);
    static enum NodeColors get_opponent_color(int code);

    static char decode(int code);
    static enum NodeColors encode(char code);
    
    static const char *get_color_name(int code);
};

# endif

// Author: Pavel Kharyuk, 2024