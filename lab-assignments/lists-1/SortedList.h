#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>
#include <assert.h>

/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just asume
  some type T was previously defined.
*/

// -----------------------------------------------------------------------------

typedef struct SortedList {
    T value;
    struct SortedList* next;
} SortedList;

// TODO : Cerința 1 & Cerința 2

void init(SortedList** slist) {
    (*slist) = NULL;
}

int isEmpty(SortedList* slist) {
    return (slist == NULL);
}

int contains(SortedList* slist, T value) {
    SortedList* head = slist;
    while (head != NULL) {
        if (head->value == value) {
            return 1;
        }
        head = head->next;
    }
    return 0;
}

void insert(SortedList** slist, T value) {
    SortedList* l;
    if (((*slist) == NULL) || ((*slist)->value > value)) {
        l = (SortedList*)malloc(sizeof(SortedList));
        l->next = (*slist);
        l->value = value;
        (*slist) = l;
    }
    else {
        insert(&((*slist)->next), value);
    }
}

void deleteOnce(SortedList** slist, T value) {

    SortedList* temp = *slist;
    SortedList* prev;
 
    // daca trebuie sa eliminam head
    if (temp != NULL && temp->value == value)
    {
        *slist = temp->next;   
        free(temp);               
        return;
    }

    while (temp != NULL && temp->value != value)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    prev->next = temp->next;

    free(temp); 

}

long length(SortedList* slist) {
    SortedList* head = slist;
    int length = 0;
    while (head != NULL) {
        length++;
        head = head->next;
    }
    return length;
}

long getNth(SortedList* slist, long N) {
    SortedList* head = slist;
    long current = 1;
    while (head != NULL) {
        if (N == current) {
            return head->value;
        }
        current++;
        head = head->next;
    }
}

void freeSortedList(SortedList** slist) {
    SortedList* head = (*slist);
    SortedList* temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
#endif

