#include <stdio.h>
#define TRUE (0 == 0)
#define FALSE (0 == 1)

int main()
{
    /*int number;
    
    printf("Enter a number: ");
    scanf("%d", &number);

    for(int i = 1; i <= number; i++)
    {
        for(int c = 1; c <= i; c++)
        {
            printf("%d", i);
        }
        printf("\n");
    }*/
    
    //-------------------------------------------
    
    /*int number_1;
    int number_2;

    printf("Enter number_1, number_2: ");
    scanf("%d,%d", &number_1, &number_2);
    
    int n;

    for(int i = number_1 + 1; i < number_2; i++)
    {
        n += i;
    }
    
    printf("%d", n);*/
    
    //-------------------------------------------
    
    /*int num_1;
    int num_2;
    int temp;

    printf("Enter two numbers: ");
    scanf("%d %d", &num_1, &num_2);
    
    if(num_1 > num_2)
    {
        temp = num_1;
        num_1 = num_2;
        num_2 = temp; 
    }
    
    int sum = 0;

    int product_sum = 1;

    for(int i = num_1 + 1;i < num_2; i++)
    {
        if(i %2 == 0)
        {
            sum += i;
        }
        else
        {
            product_sum *= i;
        }
    }

    printf("The sum of the even numbers is %d\n", sum);
    printf("The product of the odd numbers is %d", product_sum);*/
    
    //--------------------------------------------------
   
    /*unsigned int n = rand() % 10;
    int number;
    printf("The random number is %d, %d\n", n, rand2);
    
    int i = 0;
    
    do
    {
        printf("Enter number: ");
        scanf("%d", &number);
        i++;

        if(number == 0)
        {
            printf("The input has been terminated!");
        } 

    } while(number != 0 && i < n);*/

    //------------------------------------------------
        
    /*void f(float x)
    {
        float y = x*x -4; 
        printf("%.2f\n", y);
    }

    int num_1;
    int num_2;
    int temp;
    const float INCREMENTION = 0.01;
    
    printf("Enter two numbers: ");
    scanf("%d %d", &num_1, &num_2);
    
    // Check input data
    if(num_2 < num_1)
    {
        temp = num_1;
        num_1 = num_2;
        num_2 = temp; 
    }
    
    float x;

    for(x = num_1 + 1; x < num_2;x += INCREMENTION)
    {
        f(x);
    }*/

    return TRUE;
}