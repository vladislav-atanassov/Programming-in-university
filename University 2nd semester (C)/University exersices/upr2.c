#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node 
{
    int data;
    struct node *next;
} *head;

void insert_to_beginning(int number)
{    
    struct node *new_node = (struct node*)malloc(sizeof(struct node));

    new_node->data = number;

    new_node->next = head;

    head = new_node;
}

int get_lenght(struct node *ptr)
{
    int length = 0;

    while(ptr != NULL)
    {
        ptr = ptr->next;
        length++;
    }

    return length;
}

void insert_pos(struct node *ptr, int pos, int number)
{
    struct node *new_node = (struct node*)malloc(sizeof(struct node));

    new_node->data = number;
    
    int i;
    struct node *temp;
    
    if(head == NULL)
    {
        perror("\nThe list is empty!");
        exit(1);
    }
    else if((pos < 0) || (pos > get_lenght(head)))
    {
        perror("\nThere is no such position!");
        exit(1);
    }
    else
    {
        if(pos == 0)
        {
            insert_to_beginning(pos);
        }
        else
        {
            ptr = head;

            for(i = 0; i < pos-1; i++)
            {
                ptr = ptr->next;
            }
        }
    }

    new_node->next = ptr->next;
    ptr->next = new_node;    
}

void create()
{
    int number, i, value_of_node;
    printf("Enter number of nodes: ");
    scanf("%d", &number);
    
    for(i = 0; i < number; i++ )
    {
        printf("Enter a value for the node: ");
        scanf("%d", &value_of_node);
        insert_to_beginning(value_of_node); 
    }
}  

void delete_beginning(struct node* ptr)
{
    ptr = head;  
    head = ptr->next;  

    free(ptr);
}

void delete_position(struct node* ptr, int pos)
{
    int i;
    struct node *temp;
    
    if(head == NULL)
    {
        perror("\nThe list is empty!");
        exit(1);
    }
    else if((pos < 0) || (pos > get_lenght(head)))
    {
        perror("\nThere is no such position!");
        exit(1);
    }
    else
    {
        if(pos == 0)
        {
            delete_beginning(head);
        }
        else
        {
            ptr = head;

            for(i = 0; i < pos; i++)
            {
                temp = ptr;
                ptr = ptr->next;
            }
        }
    }

    temp->next = ptr->next;
    free(ptr);
}


void print_linked_list(struct node* ptr)
{
    if(head == NULL)
    {
        perror("The list is empty!");
        exit(1);
    }
    
    printf("\nLinked list is: ");

    ptr = head;
    
    while(ptr != NULL)
    {
        printf("%d", ptr->data);
        ptr = ptr->next;

        if(ptr != NULL)
        {
            printf("=>");
        }
    }
}

int main()
{
    head = NULL;
    struct node *temp;
    
    create();

    print_linked_list(head);
    
    insert_to_beginning(18);

    print_linked_list(head);
    
    insert_pos(head, 2, 19);

    print_linked_list(head);

    delete_position(head, 2);

    FILE *file = fopen("pisane.txt", "w");

    temp = head;

    while(temp != NULL)
    {
        fprintf(file, "%d ", temp->data);
        temp = temp->next;
    }

    fclose(file);

    file = fopen("pisane.txt", "r");

    temp = head;

    while(temp != NULL)
    {
        fscanf(file, "%d", &temp->data);
        temp = temp->next;
    }

    fclose(file);

    print_linked_list(head);

    return 0;
}