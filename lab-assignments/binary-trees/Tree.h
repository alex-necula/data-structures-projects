#ifndef TREE_H_D
#define TREE_H_

#include <stdio.h>
#include <stdlib.h>

typedef int Item;

typedef struct Link
  {
    Item  elem;
    struct Link *l;
    struct Link *r;
  } TreeNode;

void Init(TreeNode **t, Item x)
{
   *t = (TreeNode*)malloc(sizeof(TreeNode));
   if ((*t) == NULL) {
     printf("eroare alocare");
     return;
   }
   (*t)->l = (*t)->r = NULL;
   (*t)->elem = x;
}

void Insert(TreeNode **t, Item x)
{
    if ((*t) == NULL) {
      Init(t, x);
      return;
    } else {
      if (x <= (*t)->elem) {
        Insert(&((*t)->l), x);
      } else {
        Insert(&((*t)->r), x);
      }
    }
}

void PrintPostorder(TreeNode *t)
{
  if (t == NULL) {
 	  return;
  }
  PrintPostorder(t->l);
  PrintPostorder(t->r);
  printf("%d ", t->elem);
}

void PrintPreorder(TreeNode *t)
{
  if (t == NULL) {
    return;
  }
  printf("%d ", t->elem);
  PrintPreorder(t->l);
  PrintPreorder(t->r);
}

void PrintInorder(TreeNode *t)
{
  if (t == NULL) {
 	  return;
  }
  PrintInorder(t->l);
  printf("%d ", t->elem);
  PrintInorder(t->r);
}

void Free(TreeNode **t)
{
  if ((*t) == NULL) {
    return;
  }
  Free(&((*t)->l));
  Free(&((*t)->r));
  free((*t));
}

int Size(TreeNode* t)
{
  if (t == NULL) {
    return 0;
  }
  return Size(t->l) + 1 + Size(t->r);
}

int maxDepth(TreeNode *t)
{
  if (t == NULL) {
    return 0;
  }
  int l_depth = maxDepth(t->l) + 1;
  int r_depth = maxDepth(t->r) + 1;

  if (l_depth > r_depth) {
    return l_depth;
  } else {
    return r_depth;
  }
}

#endif // LINKEDSTACK_H_INCLUDED
