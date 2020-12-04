#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode{
	Item elem;
	struct QueueNode *next;
}QueueNode;

typedef QueueNode TcelQ;
typedef QueueNode *ACelQ;

typedef struct Queue{
	QueueNode *front;
	QueueNode *rear;
	long size;
}Queue;

typedef Queue TCoada;
typedef Queue *AQ;


Queue* createQueue(void){
	Queue* q = (Queue*) malloc(sizeof(Queue));
  if (q == NULL) {
    printf("eroare alocare");
    return NULL;
  }
  q->rear = q->front = NULL;
  q->size = 0;
	return q;
}

int isQueueEmpty(Queue *q) {
  return (q == NULL || q->front == NULL);
}

void enqueue(Queue *q, Item elem){
	QueueNode* node = (QueueNode*) malloc(sizeof(QueueNode));
  if (node == NULL) {
    printf("eroare alocare");
    return;
  }
  node->elem = elem;
  node->next = NULL;
  if (isQueueEmpty(q)) {
    q->rear = q->front = node;
  } else {
    q->rear->next = node;
    q->rear = node;
  }
  q->size++;
}

Item front(Queue* q){
  if (q != NULL) {
    return q->front->elem;
  } else {
    return 0;
  }
}

void dequeue(Queue* q){
	if (isQueueEmpty(q)) {
    return;
  }
  QueueNode* aux = q->front;
  q->front = q->front->next;
  free(aux);
  if (q->front == NULL) {
    q->rear = NULL;
  }
  q->size--;
}

void destroyQueue(Queue *q){
	while (!isQueueEmpty(q)){
    dequeue(q);
  }
  free(q);
}

#endif
