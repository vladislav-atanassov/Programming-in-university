#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERTICES 9

struct graph 
{
    char name[20];
    int distance;
    struct graph *next;
};

typedef struct graph Graph;

Graph * create_graph(int graph[VERTICES][VERTICES]) 
{
    Graph *adjMatrix[VERTICES][VERTICES];  // Initialize empty adjacency matrix

    // Initialize all values in the matrix to NULL
    for (int i = 0; i < VERTICES; i++) 
    {
        for (int j = 0; j < VERTICES; j++) 
        {
            adjMatrix[i][j] = NULL;
        }
    }

    // Populate the adjacency matrix with the graph data
    for (int i = 0; i < VERTICES; i++) 
    {
        for (int j = 0; j < VERTICES; j++) 
        {
            if (graph[i][j] != 0) 
            {
                Graph *new_node = (Graph*) malloc(sizeof(Graph));
                new_node->distance = graph[i][j];
                sprintf(new_node->name, "%d", j);
                new_node->next = adjMatrix[i][j];
                adjMatrix[i][j] = new_node;
            }
        }
    }
}

void free_allocated_memory(int graph[VERTICES][VERTICES])
{
    // Free the memory allocated for the graph
    
    for (int i = 0; i < VERTICES; i++) 
    {
        for (int j = 0; j < VERTICES; j++) 
        {
            Graph *curr_node = adjMatrix[i][j];
            free(adjMatrix[i][j]);
        }
    }
}