#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "DequeStruct.h"

Deque* dequeInIt(void)
{
	Deque* deque;

	deque = malloc(sizeof(Deque));
	deque->first = NULL;
	deque->last = NULL;

	return deque;
}

bool isEmpty(Deque* deque) { return !deque->first || !deque->last; }

void pushFront(Deque* deque, int data)
{
	DequeNode* node = malloc(sizeof(DequeNode));

	node->data = data;
	node->prev = NULL;
	node->next = deque->first;

	if(isEmpty(deque))
		deque->last = node;
	else
		deque->first->prev = node;

	deque->first = node;
}

void pushBack(Deque* deque, int data)
{
	DequeNode* node = malloc(sizeof(DequeNode));

	node->data = data;
	node->prev = deque->last;
	node->next = NULL;

	if(isEmpty(deque))
		deque->first = node;
	else
		deque->last->next = node;

	deque->last = node;
}

void pull(Deque* deque)
{
	DequeNode* node;

	int data;

	if(isEmpty(deque))
		return;

	node = deque->first;
	deque->first = node->next;

	if(!deque->first)
		deque->last = NULL;
	else
		deque->first->prev = NULL;

	data = node->data;
	free(node);
}

void pop(Deque* deque)
{
	DequeNode* node;

	int data;

	if(isEmpty(deque))
		return;

	node = deque->last;
	deque->last = node->prev;

	if(!deque->last)
		deque->first = NULL;
	else
		deque->last->next = NULL;

	data = node->data;
	free(node);
}

int peekFront(Deque* deque)
{
	return isEmpty(deque) ? deque->first->data : -1;
}

int peekBack(Deque* deque)
{
	return isEmpty(deque) ? deque->last->data : -1;
}

void print(Deque* deque)
{
	while(deque->last->next != deque->first)
	{
		printf("%d ", deque->first->data);
		deque->first = deque->first->next;
	}
}