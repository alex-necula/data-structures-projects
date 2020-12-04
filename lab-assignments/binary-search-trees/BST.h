#ifndef BST_H_
#define BST_H_

#include <stdlib.h>
/*
  IMPORTANT!

  As we stick to pure C, we cannot use templates. We will just assume
  some type T was previously defined.
 */

// -----------------------------------------------------------------------------

/*
 * !!!!ATENTION!!!!
 * 1. This implementation uses two sentinels (dummy nodes) root and nil.
 * 2. The convetion is that we always root the tree at root->l (the left of the root sentinel)
 */

typedef struct BSTNode{
	Item elem;
	struct BSTNode *p; // parent - not really needed, but simplifies implementation.
	struct BSTNode *l; // left child.
	struct BSTNode *r; // right child.
}BSTNode;

typedef struct BSTree{
	long size; // This inintially is 0, and is updated by insert/delete
	BSTNode *root; // The sentinel root (a sentinel not the actual root). The actual root node is root->l !
	BSTNode *nil; // The nil sentinel to avoid checks for NULL
}BSTree;

BSTree* createTree(void) {
  BSTree* newTree = (BSTree*) malloc(sizeof(BSTree));

  newTree->nil = (BSTNode*) malloc(sizeof(BSTNode));
  newTree->nil->p = newTree->nil->l = newTree->nil->r = newTree->nil;

  newTree->root = (BSTNode*) malloc(sizeof(BSTNode));
  newTree->root->p = newTree->root->l = newTree->root->r = newTree->nil;

  newTree->size = 0;

  return newTree;
}

int isEmpty(BSTree* tree) {
  return (tree->root->l == tree->nil);
}

void insert(BSTree* tree, Item elem) {
  BSTNode* x = tree->root->l;
  BSTNode* y = tree->root;

  BSTNode* newNode = (BSTNode*) malloc(sizeof(BSTNode));
  newNode->elem = elem;
  newNode->l = newNode->r = tree->nil;

  while (x != tree->nil) {
    y = x;
    if (newNode->elem < x->elem) {
      x = x->l;
    } else if (newNode->elem > x->elem) {
      x = x->r;
    } else {
      free(newNode);
      return;
    }
  }

  newNode->p = y;

  if ( (y == tree->root) || (newNode->elem < y->elem) ) {
    y->l = newNode;
  } else {
    y->r = newNode;
  }

  tree->size++;
}

BSTNode* search(BSTree* tree, Item key) {
  BSTNode* x = tree->root->l;

  while (x != tree->nil && key != x->elem) {
    if (key < x->elem) {
      x = x->l;
    } else {
      x = x->r;
    }
  }

  if (x == tree->nil) {
    return NULL;
  }
  return x;
}

BSTNode* minimum(BSTree* tree, BSTNode* x) {
  while (x->l != tree->nil) {
    x = x->l;
  }
  return x;
}

BSTNode* maximum(BSTree* tree, BSTNode* x) {
  while (x->r != tree->nil) {
    x = x->r;
  }
  return x;
}

BSTNode* successor(BSTree* tree, BSTNode* x) {
  if (x == NULL) {
    return NULL;
  }
  if (x->r != tree->nil) {
    return minimum(tree, x->r);
  }
  BSTNode* y = x->p;
  while (y != tree->root && x == y->r) {
    x = y;
    y = y->p;
  }
  if (y == tree->root) {
    return NULL;
  }
  return y;
}

BSTNode* predecessor(BSTree* tree, BSTNode* x) {
  if (x->l != tree->nil) {
    return maximum(tree, x->l);
  }
  BSTNode* y = x->p;
  while (y != tree->root && x == y->l) {
    x = y;
    y = y->p;
  }
  if (y == tree->root) {
    return NULL;
  }
  return y;
}

void delete(BSTree* tree, Item elem){
	BSTNode* z; // node to be deleted
	BSTNode* y; // Node that is spliced out
	BSTNode* x; // The child of the sliced out node

	// Needed to maintain a similar interface as in the previous labs
	if( (z = search(tree,elem)) == tree->nil)
		return;

	/*
	 * Note:
	 * Case 1: The node has no children
	 * Case 2: The node has one child
	 * Case 3: The node has two children
	 */

	// Are we in cases 1,2 or in case 3
	y = ( (z->l == tree->nil) || (z->r == tree->nil) ) ? z : successor(tree, z);

	// Set x to the child of y
	x = (y->l == tree->nil) ? y->r : y->l;

	/*
	 * Note:
	 * There is no need to check if x is a valid pointer, we have the sentinels!
	 */
	if (tree->root == (x->p = y->p)) // assignment of y->p to x->p is intentional
		tree->root->l = x;
	else if (y == y->p->l)
		y->p->l = x;
	else
		y->p->r = x;

	if (y != z){

		/*
		 * Note:
		 * The data type T might be a very big structure, thus copying the key
		 * from one node to another might be very inefficient. It is better to
		 * modify the actual links.
		 */

		// y is the node to splice out and x is its child
		y->l = z->l;
		y->r = z->r;
		y->p = z->p;
		z->l->p = z->r->p = y;
		if (z == z->p->l) z->p->l = y;
		else z->p->r = y;
		free(z);
	} else {
		free(y);
	}

	tree->size--;
}

void destroyTreeHelper(BSTree* tree, BSTNode* x) {
  if (x != tree->nil) {
    destroyTreeHelper(tree, x->l);
    destroyTreeHelper(tree, x->r);
    free(x);
  }
}

void destroyTree(BSTree* tree) {
  destroyTreeHelper(tree, tree->root->l);
  free(tree->root);
  free(tree->nil);
  free(tree);
}

#endif /* BST_H_ */
