#include "graph.hpp"

// partial template
template <>
Graph<uint>::Graph(const char *fpath, bool is_directed)
{
    TextFileReader reader(fpath);
    char *line = reader.read_line();
    uint n_vertices = safe_atou(line);
    delete[] line;

    this->init(n_vertices, is_directed);

    uint line_number = 1;
    while ((line = reader.read_line()) != 0) {
        uint *a = 0, n;
        string_to_uint_vec(line, &a, &n);
        switch (n) {
            case two:
                this->add_edge(a[0], a[1], 1);
                break;
            case three:
                this->add_edge(a[0], a[1], (uint64) a[2]);
                break;
            default:
                fprintf(
                    stderr,
                    "%s , line %u: expected %u or %u numbers per line\n",
                    fpath, line_number+1, two, three
                );
                exit(EXIT_FAILURE);
        }
        if (line)
            delete[] line;
        if (a)
            delete[] a;
        line_number += 1;
    }
}


template class Graph<uint>;
template class Graph<int>;