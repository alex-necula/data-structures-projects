#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "main.h"

int cmp1 (const void* a, const void* b) {
    TPair* pa = (TPair*)a;
    TPair* pb = (TPair*)b;

    return strcmp(pa->actor1Name, pb->actor1Name);
}

int cmp2 (const void* a, const void* b) {
    TPair* pa = (TPair*)a;
    TPair* pb = (TPair*)b;

    if (strcmp(pa->actor1Name, pb->actor1Name)) {
        return 0;
    } else {
        return strcmp(pa->actor2Name, pb->actor2Name);
    }
} 

void dfsB(TGraphL* graph, int v, int time, int* idx, int* low, 
    int* parent, TBridgeArray* bridges) {
    int u;
    idx[v] = time;
    low[v] = time;
    time++;
    
    TNode* it = graph->adl[v];
    while (it != NULL) {
        u = it->v;
        if (u != parent[v]) {
            if (idx[u] == -1) {
                parent[u] = v;
                dfsB(graph, u, time, idx, low, parent, bridges);
                low[v] = (low[v] < low[u]) ? low[v] : low[u];
                if (low[u] > idx[v]) {
                    bridges->arr[bridges->size].u = u;
                    bridges->arr[bridges->size].v = v;
                    bridges->size++;
                }
            } else {
                low[v] = (low[v] < idx[u]) ? low[v] : idx[u];
            }
        }
        it = it->next;
    }
}

TBridgeArray* bridge(TGraphL* graph) {
    int time = 0;
    int* idx = (int*) malloc(graph->nn * sizeof(int));
    int* low = (int*) malloc(graph->nn * sizeof(int)); 
    int* parent = (int*) malloc(graph->nn * sizeof(int)); 

    TBridgeArray* bridges = (TBridgeArray*) malloc(sizeof(TBridgeArray));
    bridges->arr = (TBridge*) malloc(graph->nn * sizeof(TBridge)); 
    bridges->size = 0;

    for (int v = 0; v < graph->nn; v++) {
        idx[v] = -1;
        low[v] = INT_MAX;
        parent[v] = -1;
    }

    for (int v = 0; v < graph->nn; v++) {
        if (idx[v] == -1) {
            dfsB(graph, v, time, idx, low, parent, bridges);
        }
    }

    free(idx);
    free(low);
    free(parent);

    return bridges;
}

void task3(TGraphL* graph, char* output_name) {
    FILE* outputFile = fopen(output_name, "w");
    
    TBridgeArray* bridges = bridge(graph);
    TPair* pair = (TPair*) malloc(graph->nn * sizeof(TPair));
    int actor1Idx, actor2Idx;
    char* actor1Name, *actor2Name;

    for (int i = 0; i < bridges->size; i++) {
        actor1Idx = bridges->arr[i].u;
        actor2Idx = bridges->arr[i].v;
        actor1Name = graph->nodeNames[actor1Idx];
        actor2Name = graph->nodeNames[actor2Idx];
        if (strcmp(actor1Name, actor2Name) < 0) {
            pair[i].actor1Name = strdup(actor1Name);
            pair[i].actor2Name = strdup(actor2Name);
        } else {
            pair[i].actor1Name = strdup(actor2Name);
            pair[i].actor2Name = strdup(actor1Name);
        }
    }

    qsort(pair, bridges->size, sizeof(TPair), cmp1);
    qsort(pair, bridges->size, sizeof(TPair), cmp2);

    // removing trailing newline from actor1Name
    for (int i = 0; i < bridges->size; i++) {
        if (strlen(pair[i].actor1Name) > 1) {
            pair[i].actor1Name = strtok(pair[i].actor1Name, "\n");
        }
    }

    fprintf(outputFile, "%d\n", bridges->size);
    for (int i = 0; i < bridges->size; i++) {
        if (strlen(pair[i].actor1Name) > 1) {
            fprintf(outputFile, "%s %s", pair[i].actor1Name, pair[i].actor2Name);
        }
    }

    for (int i = 0; i < bridges->size; i++) {
        free(pair[i].actor1Name);
        free(pair[i].actor2Name);
    }
    free(bridges->arr);
    free(bridges);
    free(pair);
    fclose(outputFile);
}