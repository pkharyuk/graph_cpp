#include "custom_types.hpp"
#include "graph.hpp"
#include "sample_graph.hpp"
#include "shortest_path.hpp"
#include "handle_cl_args.hpp"
#include "minimum_spanning_tree.hpp"

template class Graph<uint>;

typedef class Graph<uint> UIntGraph;
typedef class MinSpanningTree<uint> UIntMST;
typedef class ShortestPath<uint> UIntShortPath;
typedef bool (*mst_finder)(seed_t&, const UIntGraph&, UIntGraph **);

static const char *get_shortest_path_algorithm_name(
	enum ShortestPathAlgorithm alg_code
)
{
	switch (alg_code) {
		case ShortestPathAlgorithm::dijkstra:
			return "Dijkstra";
    	case ShortestPathAlgorithm::floyd_warshall:
			return "Floyd-Warshall";
		case ShortestPathAlgorithm::bellman_ford:
			return "Bellman-Ford";
		default:
			fprintf(stderr, "Invalid algorithm code\n");
			exit(EXIT_FAILURE);
	}
	return 0;
}

static void demo_average_shortest_path_costs(
	const UIntGraph& g
)
{
	for (
		uint algcode = 0;
		algcode < ShortestPathAlgorithm::n_shortest_path_algs;
		algcode++
	) {
		enum ShortestPathAlgorithm cur_code = (ShortestPathAlgorithm) algcode;
		const char *alg_name = get_shortest_path_algorithm_name(
			cur_code
		);
		printf(
			"\tAverage shortest path for every node (%s algorithm): \n",
			alg_name
		);
		for (uint src_node = 0; src_node < g.get_order(); src_node++) {
			double avg_shortest_path_cost = (
				UIntShortPath::compute_average_shortest_path_cost_for_node(
					g, src_node, 0, cur_code
				)
			);
			printf("Node %u: %lf\n", src_node, avg_shortest_path_cost);
		}
		printf("\n");
	}
}

static void demo_minimum_spanning_tree(
	seed_t& seed, const UIntGraph& g, mst_finder func,
	const char *weight_format_str,
	const char *mst_finder_name
)
{
	printf("\nMST: %s algorithm demo\n", mst_finder_name);
	UIntGraph *t = 0;
	bool success = func(seed, g, &t);
	if (!success) {
		printf("Error: could not evaluate a minimum spanning tree\n");
		return;
	}
	t->print_edges(weight_format_str);
	delete t;
	printf("\n");
}

// command-line interface:
// -s <seed> -n <n_nodes> -d <density> -a <min_dist> -b <max_dist>
// OR
// -s <seed> -f <fpath>
int main(int argc, char *argv[])
{
	InputArguments *input_args = parse_cl_args(argc, (const char **) argv);
	UIntGraph *g = 0;
	if (input_args->fpath != 0)
		g = new UIntGraph(input_args->fpath);
	else
		g = sample_random_undirected_uint_graph(
			input_args->seed,
			input_args->n_nodes,
			input_args->density,
			input_args->min_distance,
			input_args->max_distance
		);
	printf("Input graph: \n");
	g->print_edges(input_args->weight_format_str);
	printf("\n");

	demo_average_shortest_path_costs(*g);

	demo_minimum_spanning_tree(
		input_args->seed, *g, UIntMST::compute_mst_kruskal,
		input_args->weight_format_str, "Kruskal"
	);
	demo_minimum_spanning_tree(
		input_args->seed, *g, UIntMST::compute_mst_prim,
		input_args->weight_format_str, "Prim"
	);

	delete input_args;
	delete g;

	return 0;
}

// Author: Pavel Kharyuk, 2024