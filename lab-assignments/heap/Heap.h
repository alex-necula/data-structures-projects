#ifndef __HEAP_H__
#define __HEAP_H__

#include <stdlib.h>
#include <math.h>

/* We assume there is a defined primitive type Item. */
typedef struct {
	int prior; // Item priority
	Item data; // Item d
}ItemType;

typedef struct heap{
	long maxHeapSize; // capacity
	long size; // number of elements
	ItemType *elem; // array of elements
} PriQueue, *APriQueue;


PriQueue* makeQueue(int maxHeapSize){
	PriQueue* h = (PriQueue*) malloc(sizeof(PriQueue));
	h->elem = (ItemType*) malloc(maxHeapSize * sizeof(ItemType));
	h->maxHeapSize = maxHeapSize;
	h->size = 0;
	return h;
}

int getLeftChild(int i){
	return ((i << 1) + 1);
}

int getRightChild(int i) {
	return ((i + 1) << 1);
}

int getParent(int i) {
	return ((i - 1) >> 1);
}

ItemType getMax(APriQueue h) {
	return h->elem[0];
}

void siftUp(APriQueue h, int idx) {
	int parent = getParent(idx);

	while (parent >= 0 && h->elem[parent].prior < h->elem[idx].prior) {
		ItemType aux = h->elem[parent];
		h->elem[parent] = h->elem[idx];
		h->elem[idx] = aux;

		idx = parent;
		parent = getParent(idx);
	}
}


void insert(PriQueue *h, ItemType x) {
	if (h->size == h->maxHeapSize) {
		h->maxHeapSize <<= 1;
		h->elem = (ItemType*) realloc(h->elem, h->maxHeapSize * sizeof(ItemType));
	}

	h->elem[h->size] = x;
	siftUp(h, h->size);
	h->size++;
}

void siftDown(APriQueue h, int idx){
	int leftChild, rightChild;
	int largest = idx;

	do {
		idx = largest;

		leftChild = getLeftChild(idx);
		rightChild = getRightChild(idx);

		if (leftChild < h->size && h->elem[leftChild].prior > h->elem[largest].prior) {
			largest = leftChild;
		}
		
		if (rightChild < h->size && h->elem[rightChild].prior > h->elem[largest].prior) {
			largest = rightChild;
		}

		if (largest != idx) {
			ItemType aux = h->elem[largest];
			h->elem[largest] = h->elem[idx];
			h->elem[idx] = aux;
		}
	} while (largest != idx);
}

void removeMax(APriQueue h) {
	h->elem[0] = h->elem[h->size - 1];
	h->size--;
	siftDown(h, 0);
}

void freeQueue(APriQueue h){
	free(h->elem);
	free(h);
}

#endif

