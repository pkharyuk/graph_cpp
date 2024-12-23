template <class T>
static void swap(T *values, uint64 i, uint64 j)
{
    T tmp = values[i];
    values[i] = values[j];
    values[j] = tmp;
}

template <class T>
void shuffle(seed_t& seed, T *values, uint64 n, uint64 m)
{
    if (m == 0)
        m = n;
    srand(seed);
    uint64 n_cnt = (n - 1) / char_bitsize + 1;
    char *counter = new char[n_cnt];
    for (uint64 p = 0; p < n_cnt; p++)
        counter[p] = 0;
    for (uint64 k = 0; k < m; k++) {
        uint64 ind = random_integer_draw(seed, 0, n - 1, false);
        uint64 p = ind / char_bitsize;
        uint64 q = ind % char_bitsize;
        while ((counter[p] >> q) == 1) {
            ind = (ind + 1) % n;
            p = ind / char_bitsize;
            q = ind % char_bitsize;
        }
        counter[p] |= 1 << q;
        swap(values, k, ind);
    }
    seed = (seed_t) rand();
}

// explicit instantiation - needed in the case of oving this part
// to CPP file instead of keeping it separate
// template static void swap<uint>(uint *values, uint64 i, uint64 j);
// template void shuffle<uint>(seed_t &seed, uint *values, uint64 n, uint64 m);

// Author: Pavel Kharyuk, 2024