#include <stdio.h>
#include <stdlib.h>
#include "DequeStruct.h"

int main() 
{
    // Initialize a deque
    Deque *deque = dequeInit();

    // Test pushFront
    pushBack(deque, 1);
    pushBack(deque, 2);
    pushBack(deque, 3);
    pushBack(deque, 4);

    print(deque);

    printf("\n%d", peekBack(deque));

    // Free the deque
    free(deque);

    return 0;
}
