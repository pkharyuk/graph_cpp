# Graph: C++ implementation

## About

This is a custom C++ implementation of Graph abstraction. Includes the base
class (Graph), Shortest Path finder (Dijkstra, Bellman-Ford and Floyd-Warshall algorithms),
and Minimum Spanning Tree module (Prim and Kruskal algorithms). A graph can be imported
from a formatted text file or be sampled randomly.

## Project structure

### Makefile

Contains rules and shortcuts for several operations:

- ```make compile```: compile basic modules and the demo program
- ```make clean```: remove all compilation/build results
- ```make run``` : run demo
- ```make sample_compare``` : compare output of the demo for the sample_input with sample_output.txt


### Bash scripts

Bash shortcuts to handle docker commands and make rules. Usage:
```bash ./scripts.sh <command>``` (e.g., help).

To use it without prefixing bash, add executing rights to your user with ```chmod +x ./scripts.sh```.

### Samples

- ```sample_input.txt``` : example of an input text file; for storing $n$ edges ($u_i$, $v_i$) of undirected graph with positive integer weights $w_i$ must contain ($n+1$) lines, where the first line is a single positive number ($n$), and the following $n$ lines are space/tab separated pairs of numbers ($u_i$, $v_i$, $w_i$), $i=\overline{1, n}$.
- ```sample_output.txt``` : corresponding output (specific random seed used, see Makefile)

### Source files

- ```demo.c``` : main demo module
- ```build/``` : directory for storing object files
- ```src/``` : source code directory
    - ```common/``` : several generic routines / classes
        - ```custom_random.{h,c,t}pp``` : sampling random numbers
        - ```process_string.{h,c}pp``` : string operations
        - ```custom_types.hpp``` : type definitions + a few common numeric constants
        - ```custom_utils.hpp``` : min/max/abs + memory allocation templates
        - ```custom_sort.hpp``` : custom sorting
        - ```min_heap.{h,t}pp``` : custom min-heap implementation
    - ```graph/``` : graph abstractions and algorithms
        - ```graph.{h,t}pp``` : graph class + random graph sampler (with positive integer weights)
        - ```minimum_spanning_tree.{h,t}pp``` : MST routines [Prim, Kruskal]
        - ```shortest_path.{h,t}pp``` : Dijkstra, Floyd-Warshall, Bellman-Ford
        - ```union_find.{h,c}pp``` : helper module for Kruskal algorithm
    - ```handle_cl_args.{h,c}pp``` : handling command-line arguments
    - ```handle_input.{h,c}pp``` : handling input from a text file
    
## Comments

### No STL

Any STL capabilities were intentionally refused to be present in the sources. More precisely, I followed the guidelines from the
"Programming: An Introduction to the Profession" (3rd volume) by A.Stolyarov (in Russian).

### Exceptions

Exception handling (C++ native one) was omitted.

