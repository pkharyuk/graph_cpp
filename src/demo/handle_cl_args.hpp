#ifndef __HANDLE_CLARGS_HPP__
#define __HANDLE_CLARGS_HPP__

// Handle command-line arguments

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "custom_types.hpp"
#include "custom_random.hpp"
#include "process_string.hpp"

enum { expected_option_len = 2 };

// structure that captures the input parameters
struct InputArguments {
    seed_t seed;
    uint n_nodes;
    double density;
    uint min_distance;
    uint max_distance;
    const char *fpath;
    const char *weight_format_str;

    InputArguments(
        seed_t random_seed = 0,
        uint n = 20, double mu = 0.3,
        uint min_dist = 1, uint max_dist = 5,
        const char *file_path = 0,
        const char *wformat_str = "%2u" // uint case
    )
        : seed(random_seed)
        , n_nodes(n)
        , density(mu)
        , min_distance(min_dist)
        , max_distance(max_dist)
        , fpath(file_path)
        , weight_format_str(wformat_str)
        {}

};

InputArguments *parse_cl_args(int argc, const char **argv);

#endif

// Author: Pavel Kharyuk, 2024