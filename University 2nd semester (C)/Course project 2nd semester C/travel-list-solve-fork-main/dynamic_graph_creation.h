#ifndef DYNAMIC_GRAPH_CREATION
#define DYNAMIC_GRAPH_CREATION

#define MAX_NAME_SIZE 20

// Declaring the nodes in the graph with arguments:
// 1. The cost of the node
// 2. The name of the node
struct Node
{
    int cost;
    char *name;
};

// Declarating the graph with argumnets:
// 1. The number of vertices (size of the matrix) int ver
// 2. The adjacency matrix int **adj
struct Graph
{
    int ver;
    struct Node **adj;
};

int get_vertices(void);

int get_cost(void);

int get_position(int vertices, const char* message);

void add_edge(struct Graph* graph, int position_x, int position_y, char* name, int cost);

char* get_name(void);

struct Graph* dynamic_graph_creation(void);

void free_memory(struct Graph* graph);

#endif
