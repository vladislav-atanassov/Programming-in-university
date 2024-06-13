#ifndef DEQUESTRUCT_H
#define DEQUESTRUCT_H

#include <stdbool.h>

typedef struct DequeNode 
{
	int data;
	struct DequeNode *next;
	struct DequeNode *prev;
} DequeNode;

typedef struct Deque 
{
	struct DequeNode *first;
	struct DequeNode *last;
} Deque;

Deque *dequeInIt(void);

bool isEmpty(Deque *deque);

void pushFront(Deque *deque, int data);

void pushBack(Deque *deque, int data);

void popFront(Deque *deque);

void popBack(Deque *deque);

int peekFront(Deque *deque);

int peekBack(Deque *deque);

void print(Deque* deque);

#endif