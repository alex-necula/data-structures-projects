/*
 * BSTList.c
 *
 *  Created on: Apr 2, 2016
 *      Author: dan
 */

typedef long Item;
#include "BST.h"
typedef BSTNode* Item2;
#include "List.h"
#include "Queue.h"


BSTree* buildTree(Item *array, long len);

// -----------------------------------------------------------------------------
List* bstToList(BSTree* tree){
	List* newList = createList();
  Queue* queue;
  initQueue(&queue);

  BSTNode* listElem = tree->root->l;
  enqueue(queue, listElem);

  while (!isEmptyQueue(queue)) {
    listElem = front(queue);
    dequeue(queue);
    listPushBack(newList, listElem);
    if (listElem->l != tree->nil) {
      enqueue(queue, listElem->l);
    }
    if (listElem->r != tree->nil) {
      enqueue(queue, listElem->r);
    }
  }
  destroyQueue(queue);
  return newList;
}
void printList(List* list){
	for (ListNode* it = list->head->next; it != list->head; it = it->next) {
    printf("%ld ", it->elem->elem);
  }
  printf("\n");
}
// -----------------------------------------------------------------------------

int main(void){
	Item array[] = {5,3,2,4,7,6,8};
  List* list = createList();
	BSTree *tree = buildTree(array, sizeof(array)/sizeof(Item));
  list = bstToList(tree);
	printList(list);

	destroyTree(tree);

	return 0;
}

BSTree* buildTree(Item *array, long len){
	BSTree* newTree = createTree();
	for(int i = 0; i < len; i++){
		insert(newTree,array[i]);
	}
	return newTree;
}
