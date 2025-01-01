#ifndef __HANDLE_CLARGS_HPP__
#define __HANDLE_CLARGS_HPP__

// Handle command-line arguments

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "custom_types.hpp"
#include "game_types.hpp"
#include "custom_random.hpp"
#include "process_string.hpp"

enum { expected_option_len = 2 };

// structure that captures the input parameters
struct InputArguments {
    seed_t seed;
    uint size;
    char player_code;
    uint maxitnum;
    int bot_strategy;

    InputArguments(
        seed_t random_seed_in = 0,
        uint size_in = 11,
        char player_code_in = 'R',
        uint maxitnum_in = 1000,
        int bot_strategy_in = BotStrategy::monte_carlo_lookahead
    )
        : seed(random_seed_in)
        , size(size_in)
        , player_code(player_code_in)
        , maxitnum(maxitnum_in)
        , bot_strategy(bot_strategy_in)
        {}

};

InputArguments *parse_cl_args(int argc, const char **argv);

#endif

// Author: Pavel Kharyuk, 2024