#include <stdio.h>
#include <conio.h>

#define VERTICES 9
#define FIRST_CHAR 'A'

#define RIGHT_ARROW_KEY 77
#define LEFT_ARROW_KEY 75
#define ENTER_KEY 13

// (SPECIAL_ARROW_KEY 224) and (SPECIAL_KEY_IF_STANETEMT 0) will be used 
// because the left and right arrows are special symbols
// and if not included the getch() would also catch 'M' and 'K' chars.
// 224 works is the special symbol for my computer but on others it
// may be 0 so it is also included in the if statement

#define SPECIAL_ARROW_KEY 224       
#define SPECIAL_KEY_IF_STANETEMT 0

// Function to accept the input for a starting point
// through using the left and right arrow on the keyboard
// It always starts from the first point ('A') and by pressing the arrows
// it goes to the next one. If it has come to the end pressed right arrow at the
// last point and goes at the first and if pressed left at the first it goes to the last.
// The function takes no arguments.

// NOTE: THE FUNCTION RESPONDS ONLY TO THE FUNCTIONAL KEYS ('<-', '->', 'ENTER').

int accept_arrow_input(void)
{
    int point;
    int i = 0;

    printf("Choose starting point.\n");

    printf("Your current choice is point 'A'.\n");

    while(1) 
    {  
        point = getch();
        
        if(point == SPECIAL_ARROW_KEY || point == SPECIAL_KEY_IF_STANETEMT) // Validating that it is a special symbol
        {
            point = getch();
            
            if(point == RIGHT_ARROW_KEY)        // Catching right arrow character
            {
                i++;
                if(i > VERTICES - 1)
                {
                    i -= VERTICES;              // Go to the beginning if the choice becomes unavailable
                }
                printf("Your current choice is point '%c'.\n", FIRST_CHAR + i);
            }
            else if(point == LEFT_ARROW_KEY)    // Catching left arrow charecter
            { 
                i--;
                if(i < 0)
                {
                    i += VERTICES;              // Go to the end if the choice becomes unavailable
                }
                printf("Your current choice is point '%c'.\n", FIRST_CHAR + i);
            }
        }
        else if(point == ENTER_KEY)             // Catching ENTER character
        {
            printf("\nYou chose point '%c'.\n\n", FIRST_CHAR + i); 

            return i;                           // Return the gotten result
        }
    }
}