#include <stdio.h>
#include <limits.h> // for CHAR_BIT constant
#define EXIT_SUCCESS 0

void to_binary(void *data, size_t size)
{
    unsigned char *p = data;
    unsigned char byte;
    int i, j;
    
    printf("Binary representation of data is: ");
    
    for (i = size - 1; i >= 0; i--) 
    {
        for (j = CHAR_BIT - 1; j >= 0; j--)
        {
            byte = *(p + i);
            byte >>= j;
            if (byte & 1) 
            {
                printf("1");
            } 
            else 
            {
                printf("0");
            }
        }
    }
    
    printf("\n");
}

int main()
{
    char c = '2';
    short s = 100;
    int i = 42;
    
    to_binary(&c, sizeof(c));
    to_binary(&s, sizeof(s));
    to_binary(&i, sizeof(i));
    
    return EXIT_SUCCESS;
}
