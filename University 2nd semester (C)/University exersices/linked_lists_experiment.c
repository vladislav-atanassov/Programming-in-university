#include <stdio.h>
#include <stdlib.h>

// A structure of linked list node 

struct node 
{
    int data;
    int key;
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

int getLength(struct node *head){

    int length = 0;

    while(head != NULL)
    {
        head = head->next;
        length++;
    }

    return length;
}

//Prints a linked list from head node till the tail node

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
void insert_end()
{
    struct node *temp,*ptr;
    temp=(struct node *)malloc(sizeof(struct node));
    if(temp==NULL)
    {
        printf("\nOut of Memory Space: ");
        return;
    }
    printf("\nEnter the data value for the node: " );
    scanf("%d",&temp->data);
    
    temp->next =NULL;
    if(head==NULL)
    {
        head=temp;
    }
    else
    {
        ptr=head;
        while(ptr->next !=NULL)
        {
            ptr=ptr->next ;
        }
        ptr->next =temp;
    }
}

void insert_pos()
{
    struct node *ptr,*temp;
    int i,pos;
    temp=(struct node *)malloc(sizeof(struct node));
    if(temp==NULL)
    {
            printf("\nOut of Memory Space:n");
            return;
    }
    printf("\nEnter the position for the new node to be inserted: ");
    scanf("%d",&pos);
    printf("\nEnter the data value of the node: ");
    scanf("%d",&temp->data) ;

    temp->next=NULL;
    if(pos==0)
    {
            temp->next=head;
            head=temp;
    }
    else
    {
        for(i=0,ptr=head;i<pos-1;i++) 
        { 
            ptr=ptr->next;
            if(ptr==NULL)
            {
                printf("\nPosition not found:[Handle with care] ");
                return;
            }
        }
        temp->next =ptr->next ;
        ptr->next=temp;
    }
}

void delete_begin()
{
    struct node *ptr;
    if(ptr==NULL)
    {
        printf("\nList is Empty:n");
        return;
    }
    else
    {
        ptr=head;
        head=head->next ;
        printf("\nThe deleted element is :%d ",ptr->data);
        free(ptr);
    }
}

void delete_end()
{
    struct node *temp,*ptr;
    if(head==NULL)
    {
        printf("\nList is Empty:");
        exit(0);
    }
    else if(head->next ==NULL)
    {
        ptr=head;
        head=NULL;
        printf("\nThe deleted element is:%d ",ptr->data);
        free(ptr);
    }
    else
    {
        ptr=head;
        while(ptr->next!=NULL)
        {
            temp=ptr;
            ptr=ptr->next;
        }
        temp->next=NULL;
        printf("\nThe deleted element is:%d ",ptr->data);
        free(ptr);
    }   
}
void delete_pos()
{
    int i,pos;
    struct node *temp,*ptr;
    if(head==NULL)
    {
        printf("\nThe List is Empty:n");
        exit(0);
    }
    else
    {
        printf("\nEnter the position of the node to be deleted: ");
        scanf("%d",&pos);
        if(pos==0)
        {
            ptr=head;
            head=head->next ;
            printf("\nThe deleted element is:%d ",ptr->data  );
            free(ptr);
        }
        else
        {
            ptr=head;
            for(i=0;i<pos;i++) 
            { 
                temp=ptr; ptr=ptr->next ;
                if(ptr==NULL)
                {
                    printf("\nPosition not Found: ");
                    return;
                }
            }
            temp->next =ptr->next ;
            printf("\nThe deleted element is:%d ",ptr->data );
            free(ptr);
        }
    }
}

struct node* findMiddle(struct node* head)
{
    struct node* slowPointer = head;
    struct node* fastPointer = head;

    while (fastPointer != NULL && fastPointer->next != NULL)
    {
        slowPointer = slowPointer->next;
        fastPointer = fastPointer->next->next;
    }

    return slowPointer;
}

void sort() 
{
    int i, j, k, tempKey, tempData;
    struct node *current;
    struct node *next;
    
    int size = getLength(head);
    k = size;
   
    for ( i = 0 ; i < size - 1 ; i++, k-- ) 
    {
        current = head;
        next = head->next;
        for ( j = 1 ; j < k ; j++ ) 
        {
            if ( current->data > next->data ) 
            {
                tempData = current->data;
                current->data = next->data;
                next->data = tempData;
                tempKey = current->key;
                current->key = next->key;
                next->key = tempKey;
            }
            current = current->next;
            next = next->next;
        }
    }
}

int main() 
{
    initialize();

    /* Creating a linked List*/ 

    create();

    printLinkedList(head);

    printf("\nLinked List Length : %d", getLength(head));

    insert(5);
    
    printLinkedList(head);

    struct node* middleNode = findMiddle(head);
    printf("\nMiddle Node: %d", middleNode->data);
    printf("\nThe sorted list is: ");
    sort();

    printLinkedList(head);

    return 0;
}