#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long Item;
#include "BST.h"

// -----------------------------------------------------------------------------
BSTree* buildTree(Item *array, long len){
	BSTree* newTree = createTree();
	for(int i = 0; i < len; i++){
		insert(newTree,array[i]);
	}
	return newTree;
}
// -----------------------------------------------------------------------------
void preOrderPrint(BSTree* tree, BSTNode* x);
void inOrderPrint(BSTree* tree, BSTNode* x);
void postOrderPrint(BSTree* tree, BSTNode* x);
// -----------------------------------------------------------------------------
int isBSTUtil(BSTree* tree, BSTNode* x, long min, long max) {
  if (x == tree->nil) {
    return 1;
  }
  if (x->elem < min || x->elem > max) {
    return(0);
  }
  return isBSTUtil(tree, x->l, min, x->elem) &&
  isBSTUtil(tree, x->r, x->elem + 1l, max);
}

int isBST(BSTree* tree, BSTNode* x){
	long min = minimum(tree, x)->elem;
  long max = maximum(tree, x)->elem;
  return(isBSTUtil(tree, x, min, max));
}


// -----------------------------------------------------------------------------
int main(void) {
	Item array[] = {5,3,2,4,7,6,8};

	BSTree *tree = buildTree(array, sizeof(array)/sizeof(array[0]));

	if(isBST(tree,tree->root->l))
		printf("The tree is a binary tree!\n");
	else
		printf("The tree is NOT a binary tree!\n");

	printf("Pre-Order: ");
	preOrderPrint(tree,tree->root->l);
	printf("\n");

	printf("In-Order: ");
	inOrderPrint(tree,tree->root->l);
	printf("\n");

	printf("Post-Order: ");
	postOrderPrint(tree,tree->root->l);
	printf("\n");

	destroyTree(tree);
	return 0;
}

void preOrderPrint(BSTree* tree, BSTNode* x){
	if (x != tree->nil) {
		printf("%ld ",x->elem);
		preOrderPrint(tree,x->l);
		preOrderPrint(tree,x->r);
	}
}

void inOrderPrint(BSTree* tree, BSTNode* x){
	if (x != tree->nil) {
		inOrderPrint(tree,x->l);
		printf("%ld ",x->elem);
		inOrderPrint(tree,x->r);
	}
}

void postOrderPrint(BSTree* tree, BSTNode* x){
	if (x != tree->nil) {
		postOrderPrint(tree,x->l);
		postOrderPrint(tree,x->r);
		printf("%ld ",x->elem);
	}
}
