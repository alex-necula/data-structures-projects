#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "main.h"
#include "heap.h"

#define MAX_NAME_LENGTH 60

// reads actors' names and returns their corresponding indexes
void read_task2_data (char* input_name, TGraphL* graph, 
    int* n1, int* n2) {

    FILE* inputFile = fopen(input_name, "r");
    if (inputFile == NULL) {
        return;
    }

    char* actor1 = (char*) malloc(MAX_NAME_LENGTH * sizeof(char));
    char* actor2 = (char*) malloc(MAX_NAME_LENGTH * sizeof(char));
    char* buf = (char*) malloc(MAX_NAME_LENGTH * sizeof(char));

    // get only last 2 names
    fseek(inputFile, -2 * MAX_NAME_LENGTH, SEEK_END);
    int parity = 1;
    while (fgets(buf, MAX_NAME_LENGTH, inputFile)) {
        if (parity % 2 == 0) {
            strcpy(actor1, buf);
        } else {
            strcpy(actor2, buf);
        }
        parity++;
    }

    for (int i = 0; i < graph->nn; i++) {
        if (strcmp(actor1, graph->nodeNames[i]) == 0) {
            *n1 = i;
            break;
        }
    }
    for (int i = 0; i < graph->nn; i++) {
        if (strcmp(actor2, graph->nodeNames[i]) == 0) {
            *n2 = i;
            break;
        }
    }

    free(actor1);
    free(actor2);
    free(buf);
    fclose(inputFile);
}

int dijkstra(TGraphL* G, int n1, int n2)
{
    int v;
	int V = G->nn;
	int d[V]; // distance between nodes

    // Creating the min heap
	MinHeap* h = newQueue(V);
    h->size = V;

    // Putting all the graph elements in heap
	for (v = 0; v < V; v++) {
		d[v] = INT_MAX;
		h->elem[v] = newNode(v, d[v]);
		h->pos[v] = v;
	}

    // Make n1 the first node in minHeap
	d[n1] = 0;
	SiftUp(h, n1, d[n1]);
	
	while (!isEmpty(h)) {
		MinHeapNode* minHeapNode = removeMin(h);
		int u = minHeapNode->v;
		TNode* t = G->adl[u];

        // iterating in the adjacency list of the node
		while (t != NULL) {
			int v = t->v;

            // Update path length
			if (isInMinHeap(h, v) && d[u] + 1 < d[v]) {
				d[v] = d[u] + 1;
				SiftUp(h, v, d[v]);
			}
			t = t->next;
		}
        
        free(minHeapNode);
	}

    destroyMinHeap(h);

    // if there is no link between the nodes
    if (d[n2] < 0) {
        d[n2] = -1;
    }
	return d[n2];
}

void task2(TGraphL* graph, char* input_name, char* output_name) {
    FILE* outputFile = fopen(output_name, "w");
    int n1, n2;

    read_task2_data(input_name, graph, &n1, &n2);

    fprintf(outputFile, "%d\n", dijkstra(graph, n1, n2));

    fclose(outputFile);
}