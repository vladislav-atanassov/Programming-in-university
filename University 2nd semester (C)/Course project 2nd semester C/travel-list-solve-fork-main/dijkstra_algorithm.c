#include <limits.h>

#include "dijkstra_algorithm.h"
#include "dynamic_graph_creation.h"
#include "print_solution.h"

#define INFINITY INT_MAX

// Finding the nearest from all the neighboring nodes
// to got to the next following the greedy algorithm (Dijkstra's)
// The funcion has two arguments: 
// 1. The current information for all the distances (int dist[])
// 2. The current information of which nodes are already visited (int visited[]) 
int min_distance(int dist[], int visited[], int vertices) 
{
    int min = INFINITY;
	int min_index, i;

    for(i = 0; i < vertices; i++) 
    {
        // If the node is not visited and the current distance
        // is shorter assign it to the new value
        if((visited[i] == 0) && (dist[i] <= min)) 
        {
            min = dist[i];
            min_index = i; // Get the index of the current nearest node
        }
    }

    return min_index;
}

// Function that finds the shortest path from a source node 
// (The source node gets chosen by the user through the accept_arrow_input() function)
// to all nodes in the graph (adjacency matrix) using the Dijkstra's algorithm (type of greedy algorithm).
// The function has two arguments: 
// 1. The already initialized graph (const int graph)
// 2. The source node (int source)
void dijkstra_algorithm(struct Graph* graph, int source) 
{
	int visited[graph->ver];  // Keeps track of that if the node has been visited or not (0, 1)
    int parent[graph->ver];   // Keeps the index of the minimum path. Then to be used in "showing_path()"
    int dist[graph->ver];     // Keeps the current distances to each point and updates if found shorter path

	// Assigning all the nodes as INFINITY
    for(int i = 0; i < graph->ver; i++) 
    {
        dist[i] = INFINITY;
        visited[i] = 0;
        parent[i] = -1;
    }

	// Consider the source node as visited
    dist[source] = 0;

    // Dijkstra's algorithm for finding the shortest path
    for (int count = 0; count < graph->ver - 1; count++) 
    {
        int min_dist = min_distance(dist, visited, graph->ver);
        visited[min_dist] = 1;    // Assign the current node as visited
        
        for(int i = 0; i < graph->ver; i++)
		{
            // First if statement: 
            // If the node has not been visited and it has a connection to the current one

            // Second if statement:
            // If the distance is not INFINITY and the current distance is shorter than the previous one 
            if((!visited[i] && graph->adj[min_dist][i].cost) && 
                (dist[min_dist] != INFINITY) && 
                (dist[min_dist] + graph->adj[min_dist][i].cost < dist[i])) 
            {
                // Assign the distance as the current distance
                dist[i] = dist[min_dist] + graph->adj[min_dist][i].cost;    
                
                // Assign the current parent with the 
                // index of the nearest neighboring node
                parent[i] = min_dist;                        
            }
        }
    }

    print_solution(graph, parent, dist, source);
}