#include "custom_random.hpp"

// Draw from Cat(a, b) (equiprobable outcomes: a, a+1, .., b-1, b)
int random_integer_draw(seed_t& seed, int a, int b, bool use_seed)
{
    if (a > b) {
        fprintf(
            stderr,
            "random_integer_draw: invalid arguments (a > b)"
        );
        exit(EXIT_FAILURE);
    }
    if (use_seed)
        srand(seed);
    int rv = a + rand() / (RAND_MAX / (b - a + 1));
    if (use_seed)
        seed = (seed_t) rand();
    return rv;
}

// vectorized version of the random_integer_draw
int* vec_random_integer_draw(seed_t& seed, uint n, int a, int b)
{
    int *v = new int[n];
    srand(seed);
    for (uint i = 0; i < n; i++)
        v[i] = random_integer_draw(seed, a, b, false);
    seed = (seed_t) rand();
    return v;
}

// U[0, 1]
double random_uniform_draw(seed_t& seed, bool use_seed)
{
    if (use_seed)
        srand(seed);
    double rv = ((double) rand()) / RAND_MAX;
    if (use_seed)
        seed = (seed_t) rand();
    return rv;
}

// vectorized random_uniform_draw
double* vec_random_uniform_draw(seed_t& seed, uint n)
{
    double *v = new double[n];
    srand(seed);
    for (uint i = 0; i < n; i++)
        v[i] = random_uniform_draw(seed, false);
    seed = (seed_t) rand();
    return v;
}

// Author: Pavel Kharyuk, 2024