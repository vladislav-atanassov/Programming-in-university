#include <stdio.h>
#define EXIT_SUCCESS 0

void char_to_binary(char* pointer)
{
    printf("Binary representation of %d is ", *pointer);
    for (int i = 0; i < 8; i++)
    {
        int bit = (*pointer & (1 << (7 - i))) != 0;
        printf("%d", bit);
    }
    printf("\n");
}

void short_to_binary(short* pointer)
{
    printf("Binary representation of %d is ", *pointer);
    for (int i = 0; i < 16; i++)
    {
        int bit = (*pointer & (1 << (15 - i))) != 0;
        printf("%d", bit);
    }
    printf("\n");
}

void int_to_binary(int* pointer)
{
   printf("Binary representation of %d is ", *pointer);
    
    int mask = 1 << 31;
        
    /*for (int i = 0; i < 32; i++)
    {
        if ((*pointer & mask) == 0) {
            printf("0");
        } else {
            printf("1");
        }
        *pointer = *pointer << 1;
    }
    printf("\n");*/

    for (int i = 0; i < 32; i++)
    {
        int bit = (*pointer & (1 << (31 - i))) != 0;
        printf("%d", bit);
    }
    printf("\n");
}

int main() {
    int number = 10;
    int_to_binary(&number);
    
    char c = 'A';
    char_to_binary(&c);
    
    short s = 100;
    short_to_binary(&s);
    
    int num = 42;
    int_to_binary(&num);
    
    return EXIT_SUCCESS; 
}
