#include <stdio.h>
#include <string.h>
#include <math.h>

void num_split(int n);

int main()
{
    int n;
    printf("Enter number: ");
    scanf("%d", &n);
    num_split(n);
    return 0;
}

void num_split(int n)
{
    int m = log10(abs(n))+1;
    int sign = (n >= 0) ? 1 : -1;
    int result[m];
    
    for (int i = 0; i < m; i++)
    {
        int digit = abs(n) % 10;
        result[m-i-1] = sign * digit * pow(10, i);
        n /= 10;
    }
    
    for (int i = 0; i < m; i++)
    {
        printf("%d ", result[i]);
    }
}
