#ifndef DIJKSTRA_ALGORITHM
#define DIJKSTRA_ALGORITHM

#include "dynamic_graph_creation.h"

int min_distance(int dist[], int visited[], int vertices);

void dijkstra_algorithm(struct Graph* graph, int source);

#endif
