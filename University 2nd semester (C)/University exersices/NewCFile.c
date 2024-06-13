#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int get_pos_x(int vertices)
{
    int position_x;

    while(true)
    {
        printf("Enter position for the edge on the X axis: ");
        scanf("%d", &position_x);
        
        // Validating out of bounds error
        if(position_x > 0 && (position_x < (vertices - 1))) 
        {
            return position_x;
        }

        printf("Ivalid input for X axis!\n");      
    }
}

int main()
{
    get_pos_x(5);

    return 0;
}