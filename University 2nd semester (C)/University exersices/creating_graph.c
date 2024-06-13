#include <stdio.h>

void creating_graph()
{
    graph[0][0] = 0;
	graph[0][1] = 0;
	graph[0][2] = 1;
	graph[0][3] = 2;
	graph[0][4] = 0;
	graph[0][5] = 0;
	graph[0][6] = 0;   
	
	graph[1][0] = 0;
	graph[1][1] = 0;
	graph[1][2] = 2;
	graph[1][3] = 0;
	graph[1][4] = 0;
	graph[1][5] = 3;
	graph[1][6] = 0;   
	
	graph[2][0] = 1;
	graph[2][1] = 2;
	graph[2][2] = 0;
	graph[2][3] = 1;
	graph[2][4] = 3;
	graph[2][5] = 0;
	graph[2][6] = 0;   
	
	graph[3][0] = 2;
	graph[3][1] = 0;
	graph[3][2] = 1;
	graph[3][3] = 0;
	graph[3][4] = 0;
	graph[3][5] = 0;
	graph[3][6] = 1;   
	
	graph[4][0] = 0;
	graph[4][1] = 0;
	graph[4][2] = 3;
	graph[4][3] = 0;
	graph[4][4] = 0;
	graph[4][5] = 2;
	graph[4][6] = 0;   
	
	graph[5][0] = 0;
	graph[5][1] = 3;
	graph[5][2] = 0;
	graph[5][3] = 0;
	graph[5][4] = 2;
	graph[5][5] = 0;
	graph[5][6] = 1;   
	
	graph[6][0] = 0;
	graph[6][1] = 0;
	graph[6][2] = 0;
	graph[6][3] = 1;
	graph[6][4] = 0;
	graph[6][5] = 1;
	graph[6][6] = 0; 
}