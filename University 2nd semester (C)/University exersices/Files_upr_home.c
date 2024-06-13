#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 5

int main()
{
    FILE *fp;
    int arr[SIZE];

    fp = fopen("num.txt", "a");
    if (fp == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }

    for (int i = 0; i < SIZE; i++)
    {
        printf("Enter a number: ");
        scanf("%d", &arr[i]);
        fprintf(fp, "%d ", arr[i]);
    }

    fclose(fp);

    fp = fopen("num.txt", "r");
    if (fp == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }

    for (int i = 0; i < SIZE; i++)
    {
        fscanf(fp, "%d", &arr[i]);
    }

    fclose(fp);

    return 0;
}
