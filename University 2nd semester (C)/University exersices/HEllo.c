#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Point
{
    int x_coodrinate;
    int y_coordinate;
    char *name;
};

struct Graph
{
    int ver;
    struct Point **point;
} graph;

struct Graph* dynamic_graph_creation(int vertices)
{
    int i;

    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->ver = vertices;

    graph->point = malloc(vertices * sizeof(struct Point*));

    for (i = 0; i < vertices; i++) 
    {
        graph->point[i] = calloc(vertices, sizeof(struct Point));
    }



    return graph;
}

int main()
{
    struct Graph* graph = dynamic_graph_creation(5);

    // Free dynamically allocated memory when done using it
    for (int i = 0; i < graph->ver; i++) 
    {
        free(graph->point[i]);
    }
    
    free(graph->point);
    free(graph);

    return 0;
}
