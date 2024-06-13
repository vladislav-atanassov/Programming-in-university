#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node 
{
    int data;
    struct node *next;
} *head;

void initialize()
{
    head = NULL;
}

//Inserts a node in front of a singly linked list.

void insert(int num) 
{
    //Create a new Linked List node 

    struct node* newNode = (struct node*) malloc(sizeof(struct node));

    newNode->data  = num;

    //Next pointer of new node will point to head node of linked list 

    newNode->next = head;

    //make new node as the new head of linked list 

    head = newNode;
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
        insert(value_of_node); 
    }
}

void printLinkedList(struct node *nodePtr)
{    
    printf("\nLinked list is: ");
    
    while (nodePtr != NULL) 
    {
        printf("%d", nodePtr->data);
        nodePtr = nodePtr->next;
        
        if(nodePtr != NULL)
        {
            printf("=>");
        }
    } 
}

int getLength(struct node *head)
{
    int length = 0;

    while(head != NULL)
    {
        head = head->next;
        length++;
    }

    return length;
}

int main()
{
    initialize();

    struct node *temp;

    create();
    
    FILE *file = fopen("linked_list.txt", "w");
    
    temp = head;

    while(temp != NULL)
    {
        fprintf(file, "%d ", temp->data);
        temp = temp->next;
    }

    fclose(file);

    file = fopen("linked_list.txt", "r");

    temp = head;

    while(temp != NULL)
    {
        fscanf(file, "%d", &temp->data);
        temp = temp->next;
    }

    fclose(file);

    printLinkedList(head);

    return 0;
}