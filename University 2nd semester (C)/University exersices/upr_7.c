#include <stdio.h>
#include <string.h>
#include <ctype.h> 

//void func(int arr[][3]);
//void func(int (*arr)[3]);
//void func(int* ,size_rows, size_col);
//arr[1*size_col + 2]; // 5th element

void size_of_string(char arr[]);
void coping_strings(char arr[], char arr_2[]);

int main()
{
    /*int arr[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};

    //Printing as a square array
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < sizeof(arr); j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }*/ 
    char arr[] = "gogog";
    char arr_2[] = "ggggg";
    char arr_3[] = "ggggg";
    char arr_4[100];
    int arr_nums[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};
    
    //size_of_string(arr);
    //comparing_strings(arr, arr_3);
    //comparing_strings(arr_2, arr_3);   
    coping_strings(arr,arr_4);
    //sum_diagonal(arr_nums);

    return 0;
}

void size_of_string(char arr[])
{
    int counter = 0;

    do
    {
        counter ++;
    } while (arr[counter] != '\0');
    
    return counter;
}

void comparing_strings(char arr[], char arr_2[])
{
    int counter, counter_2 = 0;
    int n, n_2 = 0;
    do
    {
        counter ++;
    } while (arr[counter] != '\0');    
    
    do
    {
        counter_2 ++;
    } while (arr[counter_2] != '\0');


    if(counter < counter_2)
    {
        int temp = counter;
        counter = counter_2;
        counter_2 = temp;
    }

    for(int i = 0; i < counter; i++)
    {
        if(arr[i] < arr_2[i])
        {
            n_2++;
        }
        else if(arr[i] > arr_2[i])
        {
            n++;
        }
        else
        {
            n, n_2 = 0;
        }
    }

    if (n == n_2)
    {
        printf("\nThe strings are equal.");
    }
    else if(n > n_2)
    {
        printf("\nThe first is bigger.");
    }
    else
    {
        printf("\nThe second is bigger.");
    }
}

void coping_strings(char arr[], char arr_2[])
{
    for(int i = 0; arr[i] != '\0';++i)
    {
        arr_2[i] = arr[i];
    }
    
    return arr_2;
}

void sum_diagonal(int arr[][])
{
    int  sum = 0;
    for(int i = 0; i < 3; i++)
    {
       sum += arr[i][i];
    }

    printf("%d", sum);
}

void reversing_capital(char arr[])
{
    for(int i = 0; i < strlen(arr); i++)
    {
        if(arr[i] >= 'A' && arr[i] < 'Z')
        {
            arr[i] = arr[i] + 32;
        }
        else if( arr[i] >= 'a' && arr[i] < 'z')
        {
            arr[i] = arr[i] - 32;
        }
    }

    printf("%s", arr);
}