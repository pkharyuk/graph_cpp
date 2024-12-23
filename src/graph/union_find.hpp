#ifndef __UNION_FIND_HPP__
#define __UNION_FIND_HPP__

#include <stdio.h>
#include <stdlib.h>
#include "custom_types.hpp"

// union-find routines (no class)
uint uf_find(uint *& preds, uint u, uint n);
void uf_union(uint *& preds, uint *& ranks, uint u, uint v, uint n);

#endif

// Pavel Kharyuk, 2024