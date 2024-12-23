#ifndef __CUSTOM_SORT_HPP__
#define __CUSTOM_SORT_HPP__

#include "custom_types.hpp"

template <class W>
void quicksort(
    seed_t& seed, W *& weights, uint *& indices, int i1, int j1
)
{
    if (i1 >= j1)
        return;
    int i = i1;
    int j = j1;
    int k = (i1 + j1) / 2;
    W pivot = weights[indices[k]];
    while (i <= j) {
        //while (i <= j && weights[indices[i]] < weights[indices[k]])
        while (weights[indices[i]] < pivot)
            i += 1;
        //while (j >= 0 && weights[indices[j]] > weights[indices[k]])
        while (weights[indices[j]] > pivot)
            j -= 1;
        if (i > j)
            break;
        if (i <= j) {
            uint tmp = indices[i];
            indices[i] = indices[j];
            indices[j] = tmp;
            i += 1;
            j -= 1;
        }
    }
    //if (i1 < j)
    quicksort(seed, weights, indices, i1, j);
    //if (i < j1)
    quicksort(seed, weights, indices, i, j1);
}

#endif
