#include <stdio.h>
#include <stdlib.h>
#define SIZE 5

void Sum(int arr[]);
void Min_el(int arr[]);
void Max_el(int arr[]);
void up_sorting(int arr[], const int size);
int nearest_to_average(int arr[]);

int main()
{
    int arr[SIZE];

    const int a, b;

    printf("Enter interval: ");
    scanf("%d %d", &a, &b);
    
    if(a > b)
    {
        int temp;
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
        
    //printf("The sorted array is: ");
    //up_sorting(arr, SIZE);
    //Sum(arr);
    //Min_el(arr);
    //Max_el(arr);
    //reverse(arr);
    //int nearest = nearest_to_average(arr);
    //printf("\nThe nearest to average is %d", nearest);
    K_sorting(arr,SIZE);

    return 0;
}   

void Sum(int arr[])
{
    int sum = 0;
    
    for(int i = 0; i < SIZE; i++)
    {
        sum += arr[i];
    }

    printf("\nThe sum is: %d", sum);
}

void Min_el(int arr[])
{
    int min = arr[0];

    for(int i = 1; i < SIZE;i++)
    {
        if(arr[i] < min)
        {
            min = arr[i];
        }
    }

    printf("\nThe smallest element is %d", min);
}

void Max_el(int arr[])
{
    int max = 0;
    int i = 0;

    for(i = 0;i < SIZE;i++)
    {
        if(arr[i] > arr[max])
        {
            max = i;
        }
    }

    printf("\nThe index of the biggest element is %d", max);
}

void reverse(int arr[])
{
    int temp;

    for(int i = 0; i < SIZE/2; i++)
    {
        temp = arr[i];
        arr[i] = arr[SIZE - i - 1];
        arr[SIZE - i - 1] = temp;
    }

    printf("\nThe reversed array is ");

    for(int i = 0; i < SIZE; i++)
    {
        printf("%d ", arr[i]);
    }
}

int nearest_to_average(int arr[])
{
    int sum = 0;
    
    for(int i = 0; i < SIZE; i++)
    {
        sum += arr[i];
    }
    
    int avg = sum/SIZE;
    int min = abs(arr[0] - avg);
    int nearest = arr[0];

    for(int i = 1; i < SIZE; i++)
    {
        int diff = abs(arr[i] - avg);
        if(diff < min)
        {
            min = diff;
            nearest = arr[i];
        }
    }

    return nearest;
}

void up_sorting(int arr[], const int size)
{
    for(int i = 0;i < size; i++)
    {
        for(int j = i; j < size; j++)
        {
            if(arr[i]>arr[j])
            {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

    for(int i = 0; i < SIZE; i++)
    {
        printf("%d ", arr[i]);
    }
}

void K_sorting(int arr[], const int size)
{
    int k, temp;
    printf("Enter K: ");
    scanf("%d", &k);  
  
    while(k)  
    {  
        temp = arr[SIZE - 1];  
        for(int i = SIZE - 1; i > 0; i--) 
        { 
            arr[i] = arr[i - 1];  
        }
        
        arr[0] = temp;  
        k--;  
    }  
  
    for(int i = 0; i < SIZE; i++)  
    {
        printf("%d ", arr[i]);  
    }
}
