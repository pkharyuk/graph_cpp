# Graph: C++ implementation

## About

This is a custom C++ implementation of Graph abstraction. Includes the base
class (Graph), Shortest Path finder (Dijkstra, Bellman-Ford and Floyd-Warshall algorithms),
and Minimum Spanning Tree module (Prim and Kruskal algorithms). A graph can be imported
from a formatted text file or be sampled randomly.

On top of the graph representation, the Hex Game is implemented. Additional information can be found at the following link:
- [https://en.wikipedia.org/wiki/Hex_(board_game)](https://en.wikipedia.org/wiki/Hex_(board_game))
- [https://www.maths.ed.ac.uk/~csangwin/hex/HexCrossBoard.pdf](https://www.maths.ed.ac.uk/~csangwin/hex/HexCrossBoard.pdf)

## Project structure

### Makefile

Contains rules and shortcuts for several operations:

- ```make compile```: compile basic modules and the demo program
- ```make clean```: remove all compilation/build results
- ```make run_demo_graph``` : run graph demo
- ```make run_hex_game``` : run hex game
- ```make sample_compare``` : compare output of the demo for the ```sample_input.txt``` with ```sample_output.txt```


### Bash scripts

Bash shortcuts to handle docker commands and make rules. Usage:
```bash ./scripts.sh <command>``` (e.g., help).

To use it without prefixing bash, add executing rights to your user with ```chmod +x ./scripts.sh```.

### Samples

- ```sample_input.txt``` : example of an input text file; for storing $n$ edges ($u_i$, $v_i$) of undirected graph with positive integer weights $w_i$ must contain ($n+1$) lines, where the first line is a single positive number ($n$), and the following $n$ lines are space/tab separated pairs of numbers ($u_i$, $v_i$, $w_i$), $i=\overline{1, n}$.
- ```sample_output.txt``` : corresponding output (specific random seed used, see Makefile)

### Source files

- ```build/``` : directory for storing object files
- ```src/``` : source code directory
    - ```common/``` : several generic routines / classes
        - ```char_buffer.{h,c}pp``` : helper structure for working with characters
        - ```custom_random.{h,c,t}pp``` : sampling random numbers
        - ```custom_sort.hpp``` : custom sorting
        - ```custom_types.hpp``` : type definitions + a few common numeric constants
        - ```custom_utils.hpp``` : min/max/abs + memory allocation templates
        - ```handle_input.{h,c}pp``` : handling input from a text file
        - ```min_heap.{h,t}pp``` : custom min-heap implementation
        - ```process_string.{h,c}pp``` : string operations
    - ```demo/``` : graph demo sources
        - ```demo_graph.cpp``` : main demo module
        - ```handle_cl_args.{h,c}pp``` : handling command-line arguments
    - ```game/``` : custom hex game implementation
        - ```board.{h,c}pp``` : game board abstraction
        - ```dialogue.{h,c}pp``` : CLI for dialogue with user
        - ```game_types.{h,c}pp``` : type definitions, constants
        - ```handle_cl_args.{h,c}pp``` : handling command-line arguments
        - ```player.{h,c}pp``` : player abstractions (user, bot)
    - ```graph/``` : graph abstractions and algorithms
        - ```graph.{h,t,c}pp``` : graph abstraction (templates)
        - ```minimum_spanning_tree.{h,t}pp``` : MST routines [Prim, Kruskal]
        - ```sample_grph.{h,c}pp``` : random graph sampler (with positive integer weights)
        - ```shortest_path.{h,t}pp``` : Dijkstra, Floyd-Warshall, Bellman-Ford
        - ```union_find.{h,c}pp``` : helper module for Kruskal algorithm
    
## Comments

### No STL

Any STL capabilities were intentionally refused to be present in the sources. More precisely, I followed the guidelines from the
"Programming: An Introduction to the Profession" (3rd volume) by A.Stolyarov (in Russian).

### Exceptions

Exception handling (C++ native one) was omitted.

