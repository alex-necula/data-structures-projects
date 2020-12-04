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
}

void insert(SortedList** slist, T value) {
    SortedList* l;
    if ((*slist) == NULL) || (*slist)->value > value) {
        l = (SortedList*)malloc(sizeof(SortedList));
        l->next = (*slist);
        l->value = value;
        (*slist) = l;
    }
    else {
        insert(&((*slist->next)), value);
    }
}

void deleteOnce(SortedList** slist, T value) {
 
    ;
}

long length(SortedList* slist) {

}

long getNth(SortedList* slist, long N) {

}

void freeSortedList(SortedList** slist) {

}
#endif

