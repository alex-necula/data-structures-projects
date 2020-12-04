#ifndef _DOUBLE_LINKED_LIST_H_
#define _DOUBLE_LINKED_LIST_H_
#include <stdlib.h>
/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just asume
  some type T was previously defined.
*/

// -----------------------------------------------------------------------------
/**
 *  Linked list representation -- R2 Curs2 (Slide 8)
 */
typedef struct ListNode{
	Item elem; // Stored node value
	struct ListNode* next; // link to next node
	struct ListNode* prev; // link to prev node
} ListNode;

/**
 *  Double linked list representation -- desen Curs 3 (Slide 19)
 */
typedef struct List{
	ListNode* first; // link to the first node
	ListNode* last; // link to the last node
}List;
// -----------------------------------------------------------------------------

/**
 * Create a new (empty)  list  -- Silde-urile 13/14 Curs 2
 *  [input]: None
 *  [output]: List*
 */
List* createList(void){
    List* list = (List*)malloc(sizeof(list));
    list->first = NULL;
    list->last = NULL;
	return list;
}
// -----------------------------------------------------------------------------


/**
 * Determines if the list is empty
 *  [input]: List*
 *  [output]: 1 - empty/ 0 - not empty
 */
int isEmpty(List *list){
    if (list->first == NULL) {
        return 1;
    }
	return 0;
}
// -----------------------------------------------------------------------------


/**
 * Determines if a list contains a specified element
 *  [input]: List*, Item
 *  [output]: int (1 - contains/ 0 - not contains)
 */
int contains(List *list, Item elem){
    ListNode* node = list->first;
    while (node != NULL) {
        if (node->elem == elem) {
            return 1;
        }
        node = node->next;
    }
	return 0;
}
// -----------------------------------------------------------------------------


/**
 * Insert a new element in the list at the specified position.
 * Note: Position numbering starts with the position at index zero
 *  [input]: List*, Item, int
 *  [output]: void
 */
void insertAt(List* list, Item elem, int pos){

	// Guard against young player errors
	if(list == NULL) return;

  ListNode* iterator = list->first;

  while (pos && iterator != NULL) {
    pos--;
    iterator = iterator->next;
  }

  if (pos == 0){
    ListNode* node = (ListNode*) malloc(sizeof(ListNode));
    node->elem = elem;
    node->prev = node->next = NULL;

    //Corner case 1: empty List
    if (isEmpty(list)){
      list->first=list->last = node;
      return;
    }
    //Corner case 2: end of List
    if (iterator == NULL){
      node->prev = list->last;
      list->last->next = node;
      list->last = node;
      return;
    }
    //Corner case 3: beginning of List
    if (iterator->prev == NULL){
      node->next = iterator;
      iterator->prev = node;
      list->first =node;
      return;
    }
    //Normal case
    node->next = iterator;
    node->prev = iterator->prev;
    iterator->prev->next = node;
    iterator->prev = node;
  }
}
// -----------------------------------------------------------------------------


/**
 * Delete the first element instance form a list.
 *  [input]: List*, Item
 *  [output]: void
 */
void deleteOnce(List *list, Item elem){
	// Guard against young player errors
	if(list == NULL) return;

  ListNode* current = list->first;
  // Corner case 1: only one element
  if (current->elem == elem && current->next == NULL) {
    list->first = NULL;
    list->last =NULL;
    free(current);
    return;
  }
  // Corner case 2: beginning of List
  if (current->elem == elem & current->next != NULL) {
    current->next->prev = NULL;
    list->first = current->next;
    free(current);
    return;
  }
  while (current!=NULL){
    if (current->elem == elem) {
      // Corner case 3: end of List
      if (current->next == NULL){
        current->prev->next = NULL;
        list->last = current->prev;
        free(current);
        return;
      }
      //Normal case
      current->next->prev = current->prev;
      current->prev->next = current->next;
      free(current);
      return;
    }
    current = current->next;
  }
}
// -----------------------------------------------------------------------------


/**
 * Compute list length
 *  [input]: List*
 *  [output]: int
 */
int length(List *list){
	// Guard against young player errors
	if(list == NULL) return 0;
  ListNode* current = list->first;
  int count = 0;
  while (current!=NULL) {
    count++;
    current=current->next;
  }
	return count;
}
// -----------------------------------------------------------------------------



/**
 * Destroy a list.
 *  [input]: List*
 *  [output]: void
 */
void destroyList(List* list){
	// Guard against young player errors
	if(list == NULL) return;
  ListNode* current = list->first;
  ListNode* temp;
  while (current->next != NULL){
    temp = current;
    current = current->next;
    free(temp);
  }
  free(current);
}
// -----------------------------------------------------------------------------


#endif //_DOUBLE_LINKED_LIST_H_
