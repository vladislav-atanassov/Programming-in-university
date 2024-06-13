#include <stdio.h>

#define VERTICES 9
#define FIRST_CHAR 'A'

// Function to print the shortest path by showing each point that has been passed
// The function has three arguments: 
// 1. All of the indexes of the nodes that were passed (int parent[])
// 2. The searched node (int end)
// 3. The already fount distances to all nodes (int distances[])

void showing_path(int parent[], int end, int distance[]) 
{
    // Checking if the node is the source node

    if (parent[end] == -1) 
	{
        printf("%c", end + FIRST_CHAR);
        return;
    }

    // Recursivly returning the function to find the next node

    showing_path(parent, parent[end], distance);

    // Printing the path

    printf(" => %c, %d;", end + FIRST_CHAR, distance[end]);
}

// Function to print the final answear for the shortest path from the source node (int source) to the searched one (int end)
// The function has three arguments: 
// 1. The source node (int source)
// 2. The searched node (int end)
// 3. The already fount distances to all nodes (int distances[])

void print_shortest_path(int source, int end, int distance[])
{
    printf("The shortest path from '%c' to '%c' is: %d.\n", 
			(source + FIRST_CHAR), (end + FIRST_CHAR), distance[end]);
}

// Function that prints the shortest path to all nodes
// It takes as searced node the current value of 'int end'
// The function has three arguments: 
// 1. All of the indexes of the nodes that were passed (int parent[])
// 2. The already fount distances to all nodes (int distances[])
// 3. The source node (int source)

void print_solution(int parent[], int distance[], int source)
{
    for(int end = 0; end < VERTICES; end++)
    {
        if(end != source)   // Do not print the path from source to source
        {
	        printf("The shortest path from '%c' to '%c' is: ", source + FIRST_CHAR, end + FIRST_CHAR);

	        showing_path(parent, end, distance);
            printf("\n");

            print_shortest_path(source, end, distance);
            printf("\n");
        }
    }
}