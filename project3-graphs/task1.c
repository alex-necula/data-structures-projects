#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "main.h"

int compare (const void* a, const void* b) {
    const char *pa = *(const char**)a;
    const char *pb = *(const char**)b;

    return strcmp(pa,pb);
}

void recursive_helper(TGraphL* graph, int nodeIndex, 
    int* productionIndex, int productionsNo) {

    // add the actor to the current production
    productionIndex[nodeIndex] = productionsNo;

    TNode* it = graph->adl[nodeIndex];
    while (it != NULL) {
        if (productionIndex[it->v] == -1) {
            recursive_helper(graph, it->v, productionIndex, productionsNo);
        }
        it = it->next;
    }
}

/* returns the productions index of each actor and number of productions
 * (connected components of graph) using recursive DFS
 */
int* productions(TGraphL* graph, int* productionsNo) {

    *productionsNo = 0;

    // initialization - array that connects each actor to a production
    int* productionIndex = (int*) malloc(graph->nn * sizeof(int));
    for (int i = 0; i < graph->nn; i++) {
        productionIndex[i] = -1;
    }

    for (int i = 0; i < graph->nn; i++) {
        if (productionIndex[i] == -1) {
            recursive_helper(graph, i, productionIndex, *productionsNo);
            (*productionsNo)++;
        }
    }

    return productionIndex;
}

void task1(TGraphL* graph, char* output_name) {
    FILE* outputFile = fopen(output_name, "w");
    int productionsNo;

    int* productionIndex = productions(graph, &productionsNo);

    // number of actors in each production
    int* actorsNo = (int*) calloc(productionsNo, sizeof(int));

    for (int i = 0; i < graph->nn; i++) {
        actorsNo[productionIndex[i]]++;
    }
    int maxActorsNo = INT_MIN;
    int maxProductionIndex;
    for (int i = 0; i < productionsNo; i++) {
        if (actorsNo[i] > maxActorsNo) {
            maxActorsNo = actorsNo[i];
            maxProductionIndex = i;
        }
    }

    char** actorNames = (char**) malloc(maxActorsNo * sizeof(char*));
    int k = 0;

    for (int i = 0; i < graph->nn; i++) {
        if (productionIndex[i] == maxProductionIndex) {
            actorNames[k] = strdup(graph->nodeNames[i]);
            k++;
        }
    }

    qsort(actorNames, maxActorsNo, sizeof(char*), compare);

    fprintf(outputFile, "%d\n", maxActorsNo);
    for (int i = 0; i < maxActorsNo; i++) {
        fprintf(outputFile, "%s", actorNames[i]);
    }

    // freeing memory
    for (int i = 0; i < maxActorsNo; i++) {
        free(actorNames[i]);
    }
    free(actorNames);
    free(productionIndex);
    free(actorsNo);
    fclose(outputFile);
}