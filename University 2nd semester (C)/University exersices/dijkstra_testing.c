#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_NAME 30

// Declarating the graph with argumnets:
// 1. The number of vertices (size of the matrix) int ver
// 2. The adjacency matrix int **adj

struct Point
{
    int x_coordinate;
    int y_coordinate;
    char *name;
};

struct Graph
{
    int ver;
    int **adj;
    struct Point **point;
} graph;

// Function to get the cost of the edge
// The funtion takes not arguments

int get_cost(void)
{
    int cost;

    

    printf("Enter cost for the edge: ");
    scanf("%d", &cost);

    if(cost < 0)
    {
        return 0;
    }
}

// Function to get the X axis position of the point
// The function takes one argument
// 1. The size of the matrix int vertices

int get_pos_x(struct Graph* graph, int vertices)
{
    int temp_x;
    
    while(1)
    {
        printf("Enter position for the edge on the X axis: ");
        scanf("%d", &temp_x);

        if(temp_x < 0 || temp_x > vertices - 1) // Validating out of index error
        {
            printf("Ivalid input for X axis!\n");
        }
        else
        {
            break;
        }   
    }

    graph->point[temp_x]->x_coordinate = temp_x;

    return temp_x;
}

int get_pos_y(struct Graph* graph, int vertices)
{
    int temp_y;

    while(1)
    {
        printf("Enter position for the edge on the Y ayis: ");
        scanf("%d", &temp_y);

        if(temp_y < 0 || temp_y > vertices - 1) // Validating out of index error
        {
            printf("Ivalid input for Y axis!\n");
        }  
        else
        {
            break;
        }   
    }

    graph->point[temp_y]->y_coordinate = temp_y;

    return temp_y;
}

char* get_name(struct Graph* graph, int pos_x, int pos_y)
{
    char* name;

    name = (char*)malloc(sizeof(char)*SIZE_NAME);
    
    printf("Enter name for the point: ");
    scanf("%s", name);
    
    graph->point[pos_x]->name = (char*)malloc(sizeof(char)*(strlen(name)+1));   // Assign the name to the Point structure
    strcpy(graph->point[pos_x]->name, name);
    
    graph->point[pos_y]->name = (char*)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(graph->point[pos_y]->name, name);
    
    return name;
}

// Function that inserts a point into the graph
// The function takes five arguments:

int add_edge(struct Graph* graph, int cost, int position_x, int position_y)
{
    // Assignning the cost of the point in the graph

    graph->point[position_x]->x_coordinate = cost;
    graph->point[position_y]->y_coordinate = cost;

    return cost;
}

// Function that creates a dynamic adjacency matrix
// The function takes one argument:
// 1. The size of the matrix int vertices

struct Graph* dynamic_graph_creation(int vertices)
{
    int i, temp_x, temp_y;

    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->ver = vertices;

    graph->point = malloc(vertices * sizeof(struct Point*));

    for (i = 0; i < vertices; i++) 
    {
        graph->point[i] = calloc(vertices, sizeof(struct Point));
    }

    int max_points = 0;

    for(i = 0; i < vertices - 1; i++)
    {
        max_points += i;
    }

    i = 0;

    do
    {
        temp_x = get_pos_x(graph, vertices);
        temp_y = get_pos_y(graph, vertices);

        if(add_edge(graph, get_cost(), temp_x, temp_y) != 0)
        {
            get_name(graph, temp_x, temp_y);
            i++;
        }
        else
        {
            break;
        }
    } while(i < max_points);

    for(i = 0; i < vertices; i++)
    {
        for(int j = 0; j < vertices; j++)
        {
            printf("%d  ", graph->point[i][j].x_coordinate);
        }
    }

    // Returning the initialized graph

    return graph;
}

// Function to free the allocated memry preventing memory leak
// The function takes one argument:
// 1. The already initialized graph (struct graph* graph)

void free_memory(struct Graph *graph)
{
    for (int i = 0; i < graph->ver; i++) 
    {
        free(graph->point[i]);
    }
    
    free(graph->point);
    free(graph);
}

int main()
{
    struct Graph* graph = dynamic_graph_creation(5);

    free_memory(graph);

    return 0;
}
