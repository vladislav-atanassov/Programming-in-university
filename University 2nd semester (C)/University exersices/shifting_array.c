#include<stdio.h>  
#include <string.h>
#define SIZE 5  
  
void array_shifting(int arr[], const int size);

int main()  
{  
    int arr[SIZE];

    const int a, b;
    int temp;

    printf("Enter interval: ");
    scanf("%d %d", &a, &b);
    
    if(a > b)
    {
        temp == b;
        b == a;
        a == temp;
    }
            
    for(int i = 0; i < SIZE; i++)
    {
        printf("Enter number: ");
        scanf("%d", &arr[i]);

        if((arr[i] < a) || (arr[i] > b))
        {
            printf("Ivalid input!\n");
            i--;
        }
    }  

    array_shifting(arr, SIZE);
  
    return 0;  
} 

void array_shifting(int arr[], const int size)
{
    int k, temp;
    printf("Enter K: ");
    scanf("%d", &k);  
        
    char direction[SIZE];
    printf("Choose a direction - right ot left: ");
    scanf("%s", direction);

    while(k)  
    {         
        if(strcmp(direction, "left") == 0)   
        {    
            //Shifting to the left
            temp = arr[0];  
            for(int i = 0; i < SIZE; i++) 
            { 
                arr[i] = arr[i + 1];  
            }
            
            arr[SIZE - 1] = temp;  
            k--;
        }
        
        else if(strcmp(direction, "right") == 0)   
        {
            //Shifting to the right
            temp = arr[SIZE - 1];
            for (int i = SIZE - 1; i > 0; i--)
            {
                arr[i] = arr[i - 1];
            }

            arr[0] = temp;
            k--;
        }

        else
        {
            printf("Invalid input!");
        }
    }  
  
    for(int i = 0; i < SIZE; i++)  
    {
        printf("%d ", arr[i]);  
    }
}   