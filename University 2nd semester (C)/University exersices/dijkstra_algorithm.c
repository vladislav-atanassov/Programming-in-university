#include <stdio.h>
#include <limits.h>

#include "accept_arrow_input.c"
#include "print_solution.c"

#ifdef VERTICES
#define VERTICES 9

#define INFINITY INT_MAX

// Finding the nearest from all the neighboring nodes
// to got to the next following the greedy algorithm (Dijkstra's)
// The funcion has two arguments: 
// 1. The current information for all the distances (int dist[])
// 2. The current information of which nodes are already visited (int visited[]) 

int minDistance(int dist[], int visited[]) 
{
    int min = INFINITY;
	unsigned int min_index, i;

    for (i = 0; i < VERTICES; i++) 
    {
        // If the node is not visited and the current distance
        // is shorter assign it to the new value

        if (visited[i] == 0 && dist[i] <= min) 
        {
            min = dist[i];
            min_index = i;  // Get the index of the current nearest node
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

void dijkstra_algorithm(const int graph[VERTICES][VERTICES], int source) 
{
	int visited[VERTICES];  // Keeps track of that if the node has been visited or not (0, 1)
    int parent[VERTICES];   // Keeps the index of the minimum path. Then to be used in "showing_path()"
    int dist[VERTICES];     // Keeps the current distances to each point and updates if found shorter path
    unsigned int i;

	// Assigning all the nodes as INFINITY

    for (i = 0; i < VERTICES; i++) {
        dist[i] = INFINITY;
        visited[i] = 0;
        parent[i] = -1;
    }

	// Consider the source node as visited

    dist[source] = 0;

    // Dijkstra's algorithm for finding the shortest path

    for (int count = 0; count < VERTICES - 1; count++) 
    {
        int min = minDistance(dist, visited);   // Get the shortest distance to a neighboring node
        visited[min] = 1;                       // Assign the current node as visited

        for (i = 0; i < VERTICES; i++) 
		{
            // First if statement: 
            // If the node has not been visited and it has a connection to the current one

            // Second if statement:
            // If the distance is not INFINITY and the current distance is shorter than the previous one 
            
            if ((!visited[i] && graph[min][i]) && ((dist[min] != INFINITY) && (dist[min] + graph[min][i] < dist[i]))) 
            {
                // Assign the distance as the current distance
                
                dist[i] = dist[min] + graph[min][i];    
                
                // Assign the current parent with the 
                // index of the nearest neighboring node
                
                parent[i] = min;                        
            }
        }
    }

	// Printing the solution 

    print_solution(parent, dist, source);
}

#endif
