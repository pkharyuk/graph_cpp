#ifndef __CUSTOM_UTILS_HPP__
#define __CUSTOM_UTILS_HPP__

// Common helper functions and utilities

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "custom_types.hpp"

// Several templates of simple general functions
template <class T>
T min(T a, T b) { return (a <= b) ? a : b; }

template <class T>
T max(T a, T b) { return (a >= b) ? a : b; }

template <class T>
T abs(T a) { return (a >= 0) ? a : -a; }

template <class T>
void resize_1d_array(T **a, uint old_size, uint new_size)
{
    T *tmp = *a;
    *a = new T[new_size];
    uint N = min<uint>(old_size, new_size);
    for (uint i = 0; i < N; i++)
        (*a)[i] = tmp[i];
    delete[] tmp;
}

template <class T>
void allocate_2d_array(
    T**& a, uint m, uint n = 0, bool fill_zeros = true
)
{
    if (a != 0) {
        fprintf(stderr, "allocate_2d_array: non-null pointer as input");
        exit(EXIT_FAILURE);
    }
    if (n == 0)
        n = m;
    a = new T*[m];
    for (uint i = 0; i < m; i++) {
        a[i] = new T[n]; // new uint[n]() ~ C++11
        if (!fill_zeros)
            continue;
        for (uint j = 0; j < n; j++)
            a[i][j] = 0;
    }
}
template <class T>
void copy_2d_array(
    T**& dst, T** const& src, uint m, uint n = 0
)
{
    if (n == 0)
        n = m;
    for (uint i = 0; i < m; i++) {
        for (uint j = 0; j < n; j++)
            dst[i][j] = src[i][j];
    }
}
template <class T>
void delete_2d_array(T **a, uint m)
{
    if (a) // nullptr
        return;
    for (uint i = 0; i < m; i++) {
        if (a[i]) // nullptr
            continue;
        delete[] a[i];
    }
    delete[] a;
}

#endif

// Author: Pavel Kharyuk, 2024