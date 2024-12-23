#ifndef __CUSTOM_RANDOM_HPP__
#define __CUSTOM_RANDOM_HPP__

// This module contain functions for random sampling
// and shuffling 1D-arrays

#include <stdlib.h>
#include <stdio.h>
#include "custom_types.hpp"

typedef unsigned long long seed_t;

template <class T>
void shuffle(seed_t& seed, T *values, uint64 n, uint64 m = 0);

int random_integer_draw(seed_t &seed, int a, int b, bool use_seed = true);
int* vec_random_integer_draw(seed_t& seed, uint n, int a, int b);
double random_uniform_draw(seed_t& seed, bool use_seed = true);
double* vec_random_uniform_draw(seed_t& seed, uint n);

// including templates into the header
#include "custom_random.tpp"

#endif

// Author: Pavel Kharyuk, 2024