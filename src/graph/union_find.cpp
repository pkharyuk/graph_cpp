#include "union_find.hpp"


uint uf_find(uint *& preds, uint u, uint n)
{
    uint cnt = 0;
    while (preds[u] != u && cnt < n) {
        cnt += 1;
        u = preds[u];
    }
    if (cnt == n) {
        fprintf(
            stderr,
            "UnionFind_Find: incorrect predecessors array passed.\n"
        );
        exit(EXIT_FAILURE);
    }
    return u;
}

void uf_union(uint *& preds, uint *& ranks, uint u, uint v, uint n)
{
    if (ranks[u] == ranks[v]) {
        preds[v] = u;
        ranks[u] += 1;
        return;
    }
    if (ranks[u] < ranks[v])
        preds[u] = v;
    else
        preds[v] = u;
    return;
}

// Author: Pavel Kharyuk, 2024
