#include <stdio.h>
#define SIZE 3

/*typedef int patka;

struct dynamic_arr
{
    int* p_arr;
    int allocated_size;
    int used_size;
};

void my_func(struct dynamic_arr* arr);

int main()
{
    struct dynamic_arr arr = {NULL, 30, 0};
    struct dynamic_arr arr2 = arr;
    struct dynamic_arr * ps = &arr;

    patka a = 3;

    (*ps).allocated_size = 40;
    ps->allocated_size = 50;

    //arr.allocated_size = 40;

    printf("%d\n", arr2.allocated_size);
    printf("%d\n", arr.allocated_size);
    printf("%d\n", a);

    printf("Hello world");

    return 0;
}*/

struct student
{
    char name[10];
    int years;
    double mark;
};

void user_input(struct student *student)
{
    printf("Enter name: ");
    scanf("%s", student->name);

    printf("Enter age: ");
    scanf("%d", &student->years);

    printf("Enter mark: ");
    scanf("%lf", &student->mark);
}

struct colour
{
    int r;
    int g;
    int b;
};

struct triangle
{
    int a;
    int b;
    int c;
    struct colour colour;
};

void longest_side(struct triangle triangle[SIZE])
{
    int max_side = triangle[0].a;

    for(int i = 0; i < SIZE - 1; i++)
    {
        if(triangle[i].a > max_side)
        {
            max_side = triangle[i].a;
        }
    }

    printf("The longest side is: %d", max_side);
}

void area(struct triangle triangle[SIZE])
{
    float area = 0;
    float half_per = 0;

    for(int i = 0; i  < SIZE - 1; i++)
    {

    }
}

int main()
{
    //struct student student;
    //
    //user_input(&student);
    //
    //printf("%s, %d, %.2lf", student.name, student.years, student.mark);

    struct triangle triangle[SIZE] = {   
                                        {3, 4, 5, {203, 23, 712}},
                                        {6, 8, 10, {342, 450, 45}},
                                        {9, 12, 15, {34, 464, 83}}
                                    };

    longest_side(&triangle);

    return 0;
}