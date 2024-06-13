#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "dynamic_graph_creation.h"

// Getting the size of the adjacency matrix
int get_vertices(void)
{
    int vertices;
    
    while(true)
    {
        printf("Enter number of nodes: ");
        scanf("%d", &vertices);

        // Checking if the matrix is valid or not
        // 1x1 matrix is not valid and negative size is not valid too
        
        if(vertices > 1)    
        {
            return vertices;
        }
        
        printf("Invalid input!\n");
    }
}

// Function to get the cost of the node
// The funtion takes not arguments
int get_cost(void)
{
    int cost;

    while(true)
    {
        printf("Enter cost for the node: ");
        scanf("%d", &cost);

        if(cost > 0)
        {
            return cost;
        }

        printf("Invalid input for cost!\n");
    }
}

// Function to get the X or Y axis position of the node
// The function takes one argument
// 1. The size of the matrix int vertices
// 2. Approapriate message for the input
int get_position(int vertices, const char* message)
{
    int position;

    while(true)
    {
        printf("Enter position for the edge on the %s: ", message);
        scanf("%d", &position);
        
        // Validating out of bounds error
        if(position >= 0 && (position < vertices))
        {
            return position;
        }

        printf("Ivalid input for %s!\n", message);     
    }
}

char* get_name(void) 
{
    char* name = (char *)malloc(sizeof(char) * MAX_NAME_SIZE); // Adjust the size as needed
    
    while(true) 
    {
        printf("Enter name for the node: ");
        scanf("%s", name);

        if(name != NULL &&
            strlen(name) > 0 && strlen(name) < MAX_NAME_SIZE) 
        {
            return name;
        }

        printf("Invalid input for name!\n");
    }
}

// Function that inserts a node into the graph
// The function takes five arguments:
void add_edge(struct Graph* graph, int position_x, int position_y, char* name, int cost)
{
    // Assignning the cost of the node in the graph
    graph->adj[position_x][position_y].cost = cost;
    graph->adj[position_y][position_x].cost = cost;
    
    // Assigning the name of the node in the graph
    graph->adj[position_x][position_y].name = name;
    graph->adj[position_y][position_x].name = name;
}

// Function that creates a dynamic adjacency matrix
// The function takes one argument:
// 1. The size of the matrix int vertices
struct Graph* dynamic_graph_creation(void) 
{
    // Allocate memory for the graph structure
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));  
    graph->ver = get_vertices(); // Assign the number of vertices in the structure

    // Allocate memory for the adjacency matrix
    graph->adj = (struct Node**)malloc(sizeof(struct Node*) * (size_t)graph->ver);
    
    // TODO: could be done with calloc() to initialize the directly to NULL
    // TODO: or make a custom function that does the initialization
    for(int i = 0; i < graph->ver; i++) 
    {
        graph->adj[i] = (struct Node*)malloc(sizeof(struct Node) * (size_t)graph->ver);
    }

    // Initialize the adjacency matrix with 0 and NULL
    for(int i = 0; i < graph->ver; i++)
    {
        for(int j = 0; j < graph->ver; j++) 
        {
            graph->adj[i][j].cost = 0;
            graph->adj[i][j].name = NULL;
        }
    }

    for(int i = 0; i < graph->ver; i++) 
    {
        add_edge(graph, get_position(graph->ver, "X axis"), 
            get_position(graph->ver, "Y axis"), get_name(), get_cost());
    }

    return graph;   // Return the initialized graph
}


// Function to free the allocated memory preventing memory leak
// The function takes one argument:
// 1. The already initialized graph (struct Graph* graph)
void free_memory(struct Graph* graph)
{
    for(int i = 0; i < graph->ver; i++) 
    {
        free(graph->adj[i]->name);
    }

    free(graph->adj);
    free(graph);
}
