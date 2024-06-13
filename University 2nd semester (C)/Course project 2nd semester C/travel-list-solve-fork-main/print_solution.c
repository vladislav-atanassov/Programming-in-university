#include <stdio.h>

#include "print_solution.h"
#include "dynamic_graph_creation.h"

// Function that finds the name of node
// The function takes two arguments:
// 1. The already initialized graph
// 2. The inxex of  the searched name
char* searching_name(struct Graph* graph, int index_of_searched_name)
{   
    int validation_index = 0;

    // Irritating the graph only over the main diagonal
    for(int i = 0; i < graph->ver; i++)
    {
        for(int j = i; j < graph->ver; j++)
        {
            if(graph->adj[i][j].cost != 0) // If the cost is 0 then there is no node in adj[i][j]
            {
                if(validation_index == index_of_searched_name)
                {
                    return graph->adj[i][j].name;
                }

                // If the validation_index is not the searched one continue to the next one
                validation_index++;
            }
        }
    }
    
    // Returning NULL pointer in case there is no such index or empty array
    return NULL; 
}

// Function to print the shortest path by showing each point that has been passed
// The function has three arguments: 
// 1. All of the indexes of the nodes that were passed (int parent[])
// 2. The searched node (int end)
// 3. The already fount distances to all nodes (int distances[])
void showing_path(struct Graph* graph, int parent[], int end, int distance[]) 
{
    // Checking if the node is the source node
    if(parent[end] == -1) 
	{
        printf("%s", searching_name(graph, end));
        return;
    }

    // Recursivly returning the function to find the next node
    showing_path(graph, parent, parent[end], distance);

    // Printing the path
    printf(" => %s, %d;", searching_name(graph, end), distance[end]);
}

// Function to print the final answear for the shortest path from the source node (int source) to the searched one (int end)
// The function has three arguments: 
// 1. The source node (int source)
// 2. The searched node (int end)
// 3. The already fount distances to all nodes (int distances[])
void print_shortest_path(struct Graph* graph, int source, int end, int distance[])
{
    printf("The shortest path from '%s' to '%s' is: %d.\n", 
			searching_name(graph, source), searching_name(graph, end), distance[end]);
}

// Function that prints the shortest path to all nodes
// It takes as searced node the current value of 'int end'
// The function has four arguments: 
// 1. All of the indexes of the nodes that were passed (int parent[])
// 2. The already fount distances to all nodes (int distances[])
// 3. The source node (int source)
// 4. The size of the matrix
void print_solution(struct Graph* graph, int parent[], int distance[], int source)
{
    for(int end = 0; end < graph->ver; end++) // Do not print the last element which will always be out of index
    {
        if(end != source)                       // Do not print the path from source to source (will always be 0)
        {
	        printf("The shortest path from '%s' to '%s' is: ", 
                    searching_name(graph, source), searching_name(graph, end));

	        showing_path(graph, parent, end, distance);
            printf("\n");

            print_shortest_path(graph, source, end, distance);
            printf("\n");
        }
    }
}

