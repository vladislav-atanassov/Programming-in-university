#include <stdio.h>

#include "print_solution.h"
#include "dynamic_graph_creation.h"
#include "dijkstra_algorithm.h"
#include "accept_arrow_input.h"

int main() 
{	
    struct Graph* graph = dynamic_graph_creation();

    dijkstra_algorithm(graph, accept_arrow_input(graph));

    free_memory(graph);

    return 0;
}
