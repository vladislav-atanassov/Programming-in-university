#ifndef PRINT_SOLUTION
#define PRINT_SOLUTION

#include "dynamic_graph_creation.h"

char* searching_name(struct Graph* graph, int index_of_searched_name);

void showing_path(struct Graph* graph, int parent[], int end, int distance[]);

void print_shortest_path(struct Graph* graph, int source, int end, int distance[]);

void print_solution(struct Graph* graph, int parent[], int distance[], int source);

#endif
