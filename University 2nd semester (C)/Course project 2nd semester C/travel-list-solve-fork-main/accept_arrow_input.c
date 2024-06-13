#include <stdio.h>
#include <stdbool.h>
#include <conio.h>

#include "accept_arrow_input.h"
#include "print_solution.h" 
#include "dynamic_graph_creation.h"

// Function to accept the input for a starting point through using the left and right arrow on the keyboard
// By pressing the arrows it goes to the next one. If it has come to the end pressed right arrow at the
// last point and goes at the first and if pressed left at the first it goes to the last.
// The function takes one argument:
// 1. The already declarated graph
// NOTE: THE FUNCTION RESPONDS ONLY TO THE FUNCTIONAL KEYS ('<-', '->', 'ENTER').
int accept_arrow_input(struct Graph* graph)
{
    int point, i = 0;
    int vertices = graph->ver - 1;  // Preventing last element not existing and returning INT_MAX distance

    printf("Choose starting point.\n");
    printf("Your current choice is point %s.\n", searching_name(graph, 0));

    while(true) 
    {  
        point = getch();
        
        if(point == SPECIAL_ARROW_KEY || point == SPECIAL_KEY_IF_STANETEMT) // Validating that it is a special symbol
        {
            point = getch();
            
            if(point == RIGHT_ARROW_KEY)        // Catching right arrow character
            {
                i++;

                if(i > vertices)
                {
                    i -= vertices + 1;              // Go to the beginning if the choice becomes unavailable
                }
                
                printf("Your current choice is point %s.\n", searching_name(graph, i));
            }
            else if(point == LEFT_ARROW_KEY)    // Catching left arrow charecter
            { 
                i--;

                if(i < 0)
                {
                    i += vertices + 1;              // Go to the end if the choice becomes unavailable
                }

                printf("Your current choice is point %s.\n", searching_name(graph, i));
            }
        }
        else if(point == ENTER_KEY)             // Catching ENTER character
        {
            printf("\nYou chose point '%s'.\n\n", searching_name(graph, i)); 
            
            return i;                           // Return the gotten result
        }
    }
}
