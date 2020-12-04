#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>



typedef struct StackNode{
	Item elem;
	struct StackNode *next;
}StackNode;

typedef StackNode TCelSt; // Redenumirea unui nod la notatiile din curs
typedef StackNode *ACelSt; // Redenumirea unui pointer la nod la notatiile din curs

typedef struct Stack{
	StackNode* head;  // Varful stivei
	long size; // Numarul de elemente din stiva
}Stack;

typedef Stack TStiva; // Redenumirea stivei la notatiile din curs
typedef Stack *ASt;//  // Redenumirea unui pointer la stiva la notatiile din curs

Stack* createStack(void){
	Stack* stack = (Stack*) malloc(sizeof(Stack));
  if (stack == NULL) {
    printf("eroare alocare");
    return NULL;
  }
	stack->head = NULL;
	stack->size = 0;
	return stack;
}

int isStackEmpty(Stack* stack){
	if (stack == NULL || stack->head == NULL) {
		return 1;
	} else {
		return 0;
	}
}

void push(Stack *stack, Item elem){
  StackNode* node = (StackNode*)malloc(sizeof(StackNode));
  node->elem = elem;
  node->next = stack->head;
  stack->head = node;
  stack->size++;
}

Item top(Stack *stack){
  if (stack != NULL && stack->head != NULL) {
    return stack->head->elem;
  }
	return -1;
}

void pop(Stack *stack){
	if (isStackEmpty(stack)) {
    return;
  }
  StackNode* temp = stack->head;
  stack->head = stack->head->next;
  stack->size--;
  free(temp);
}

void destroyStack(Stack *stack){
  StackNode* temp;
	while (!isStackEmpty(stack)) {
    pop(stack);
  }
  free(stack);
}

#endif
