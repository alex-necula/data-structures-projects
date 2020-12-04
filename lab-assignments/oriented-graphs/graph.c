#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "heap.h"

typedef int TCost;

typedef struct node
{
	int v;
	TCost c;
	struct node *next;
} TNode, *ATNode;

typedef struct
{
	int nn;
	ATNode *adl;
}	TGraphL;


void alloc_list(TGraphL * G, int n)
{
    int i;
    G->nn = n;
	G->adl = (ATNode*) malloc((n+1)*sizeof(ATNode));
	for(i = 0; i < n; i++)
		G->adl[i] = NULL;

}

void free_list(TGraphL *G)
{
    int i;
    ATNode it;
    for(i = 1; i < G->nn; i++){
		it = G->adl[i];
		while(it != NULL){
			ATNode aux = it;
			it = it->next;
			free(aux);
		}
		G->adl[i] = NULL;
	}
	G->nn = 0;
}

void insert_edge_list(TGraphL *G, int v1, int v2, int c)
{
 TNode *t;
 t=(ATNode)malloc(sizeof(TNode));
 t->v = v2; t->c=c; t->next = G->adl[v1]; G->adl[v1]=t;
 t=(ATNode)malloc(sizeof(TNode));
 t->v = v1;  t->c=c; t->next = G->adl[v2]; G->adl[v2]=t;
}


void dijkstra(TGraphL G, int s)
{
    int v, i;
	int V = G.nn;
	int d[V]; // distanta

	MinHeap* h = newQueue(V);

	for (v = 0; v < V; v++) {
		d[v] = INT_MAX;
		h->elem[v] = newNode(v, d[v]);
		h->pos[v] = v;
	}

	h->elem[s] = newNode(s, d[s]);
	h->pos[s] = s;
	d[s] = 0;
	SiftUp(h, s, d[s]);
	h->size = V;

	while (!isEmpty(h)) {
		MinHeapNode* minHeapNode = removeMin(h);
		int u = minHeapNode->v;
		TNode* t = G.adl[u];

		while (t != NULL) {
			int v = t->v;

			if (isInMinHeap(h, v) && d[u] + t->c < d[v]) {
				d[v] = d[u] + t->c;
				SiftUp(h, v, d[v]);
			}
			t = t->next;
		}
	}
	printf("\n");
	printf("Varf Drum minim \n");
	for (i = 0; i < G.nn; i++) {
		printf("%d    %d \n", i, d[i]);
	}

}

void Prim(TGraphL G)
{
    int i, v;
	int V = G.nn;
	int parent[V]; // calea
	int d[V]; // costurile

	MinHeap* h = newQueue(V);

	for (v = 1; v < V; v++) {
		parent[v] = -1;
		d[v] = INT_MAX;
		h->elem[v] = newNode(v, d[v]);
		h->pos[v] = v;
	}

	d[0] = 0;
	h->elem[0] = newNode(0, d[0]);
	h->pos[0] = 0;
	h->size = V;

	while (!isEmpty(h)) {
		MinHeapNode* minHeapNode = removeMin(h);
		int u = minHeapNode->v;
		TNode* t = G.adl[u];

		while (t != NULL) {
			int v = t->v;

			if (isInMinHeap(h, v) && t->c < d[v]) {
				d[v] = t->c;
				parent[v] = u;
				SiftUp(h, v, d[v]);
			}
			t = t->next;
		}
	}
	printf("\n");
	printf("Parinte  Varf \n");
	for (i = 1; i < G.nn; i++) {
		printf("%d        %d\n", parent[i], i);
	}
}


int main()
{
    int i,v1,v2,c;
	int V,E;
	TGraphL G;
	ATNode t;
	freopen ("graph.in", "r", stdin);
	scanf ("%d %d", &V, &E);
	alloc_list(&G, V);

	for (i=1; i<=E; i++)
	{
		scanf("%d %d %d", &v1, &v2, &c);
	    insert_edge_list(&G, v1,v2, c);
	}

	for(i=0;i<G.nn;i++)
	{
    printf("%d : ", i);
    for(t = G.adl[i]; t != NULL; t = t->next)
    printf("%d ",t->v);
    printf("\n");
	}
	dijkstra(G,0);
	Prim(G);

	return 0;
}
