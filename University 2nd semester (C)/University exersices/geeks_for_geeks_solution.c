#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define VERTICES 9
#define INFINITY 4369

int minDistance(int dist[], int visited[]) 
{
    int min = INFINITY;
	unsigned int min_index, i;

    for (i = 0; i < VERTICES; i++) 
    {
        if (visited[i] == 0 && dist[i] <= min) 
        {
            min = dist[i];
            min_index = i;
        }
    }

    return min_index;
}

char first = 'A';

void showing_path(int parent[], int end, int distance[]) 
{
    if (parent[end] == -1) 
	{
        printf("%c", end + first);
        return;
    }

    showing_path(parent, parent[end], distance);

    printf(" => %c, %d;", end + first, distance[end]);
}

int accept_arrow_input()
{
    int node;
    int i = 0;

    printf("Choose starting node.\n");

    printf("Your current choice is node 'A'.\n");

    while(1) 
    {  
        node = getch();
        int start = node;
        
        if(node == 77) // Catching right arrow character
        {
            i++;
            if(i > VERTICES - 1)
            {
                i -= VERTICES; //Go to the beginning if the choice becomes unavailable
            }
            printf("Your current choice is node %c.\n", first + i);
        }
        else if(node == 75) // Catching left arrow charecter
        { 
            i--;
            if(i < 0)
            {
                i += VERTICES; // Go to the end if the choice becomes unavailable
            }
            printf("Your current choice is node %c.\n", first + i);
        }
        else if(node == 13)
        {
            printf("\nYou chose node '%c'.\n", i + first);
            return i;
        }
    }
}

void printing_solution(int start, int end, int distance[])
{
	printf("The shortest path from %c to %c is: %d\n", 
			(start + first), (end + first), distance[end]);
}

void dijkstra_algorithm(const int graph[VERTICES][VERTICES], int start) 
{
	int visited[VERTICES];
    int parent[VERTICES];
    int dist[VERTICES];
	unsigned int i;

	// Assigning all the nodes as INFINITY

    for (i = 0; i < VERTICES; i++) {
        dist[i] = INFINITY;
        visited[i] = 0;
        parent[i] = -1;
    }

	// Consider the source node as visited

    dist[start] = 0;

    // Dijkstra's algorithm for finding the shortest path

    for (int count = 0; count < VERTICES - 1; count++) 
    {
        int min = minDistance(dist, visited);
        visited[min] = 1;

        for (i = 0; i < VERTICES; i++) 
		{
            if ((!visited[i] && graph[min][i]) && (dist[min] != INFINITY && (dist[min] + graph[min][i] < dist[i]))) 
            {
                dist[i] = dist[min] + graph[min][i];
                parent[i] = min;
            }
        }
    }

	// Printing solution 

    for(int end = 0; end < VERTICES; end++)
    {
	    printf("The shortest path from %c to %c is: ", start + first, end + first);
    
	    showing_path(parent, end, dist);
        printf("\n");

        printing_solution(start, end, dist);
        printf("\n");
    }
}

int main() 
{
	// Initializing the graph
    
    const int graph[VERTICES][VERTICES] = 
	{ 
		{ 0, 4, 0, 0, 0, 0, 0, 8, 0 },
		{ 4, 0, 8, 0, 0, 0, 0, 11, 0 },
		{ 0, 8, 0, 7, 0, 4, 0, 0, 2 },
		{ 0, 0, 7, 0, 9, 14, 0, 0, 0 },
		{ 0, 0, 0, 9, 0, 10, 0, 0, 0 },
		{ 0, 0, 4, 14, 10, 0, 2, 0, 0 },
		{ 0, 0, 0, 0, 0, 2, 0, 1, 6 },
		{ 8, 11, 0, 0, 0, 0, 1, 0, 7 },
		{ 0, 0, 2, 0, 0, 0, 6, 7, 0 } 
	};

    dijkstra_algorithm(graph, accept_arrow_input());

    return 0;
}
