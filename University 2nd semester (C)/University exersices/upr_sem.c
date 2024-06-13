#include <stdio.h>
#include <string.h>

#define SIZE 7

int main()
{
    float total_cost = 0;
    int number_of_guests;
    char user_input[SIZE];

    float chair_price = 13.99;
    float cups_price = 5.98;
    float dishes_price = 21.02;
    float table_price = 42;

    int chair = 0;
    int table = 0;
    int dishes = 0;
    int cups = 0;

    int number_of_table_seats = 8;
    int chair_seat = 0;
    int cups_number = 6;
    int dishes_number = 6;

    int number_table_chairs = 0;

    printf("Enter number of guests: ");
    scanf("%d", &number_of_guests);

    int needed_tables = number_of_guests / number_of_table_seats;
    needed_tables++;

    do
    {
        printf("Enter equipment: ");
        scanf("%s", user_input);

        if(strcmp("chair", user_input) == 0)
        {    
            chair++;
        }
        else if(strcmp("table", user_input) == 0)
        {
            table++;
        }
        else if (strcmp("cups", user_input) == 0)
        {
            cups++;
        }
        else if(strcmp("dishes", user_input) == 0)
        {
            dishes++;
        }

    } while(strcmp("party", user_input) != 0);

    total_cost = chair*chair_price + table*table_price + dishes*dishes_price + cups*cups_price;

    printf("The total cost to the moment is: %.2f", total_cost);

    return 0;
}