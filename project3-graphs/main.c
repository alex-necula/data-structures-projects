#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"

#define MAX_NAME_LENGTH 60

inputData* read_data (char* input_name) {

    int moviesNo = 0;
    FILE* inputFile = fopen(input_name, "r");
    if (inputFile == NULL) {
        return NULL;
    }

    fscanf(inputFile, "%d\n", &moviesNo);

    char** moviesNames = (char**) malloc(moviesNo * sizeof(char*));
    int* actorsNo = (int*) malloc(moviesNo * sizeof(int));
    char*** actorsNames = (char***) malloc(moviesNo * sizeof(char**));

    for (int i = 0; i < moviesNo; i++) {
        moviesNames[i] = (char*) malloc(MAX_NAME_LENGTH * sizeof(char));
        
        fgets(moviesNames[i], MAX_NAME_LENGTH, inputFile);
        fscanf(inputFile, "%d\n", &actorsNo[i]);

        actorsNames[i] = (char**) malloc(actorsNo[i] * sizeof(char*));
        for (int j = 0; j < actorsNo[i]; j++) {
            actorsNames[i][j] = (char*) malloc(MAX_NAME_LENGTH * sizeof(char));
            fgets(actorsNames[i][j], MAX_NAME_LENGTH, inputFile);
        }
    }

    inputData* data = (inputData*) malloc(sizeof(inputData));

    // packing data
    data->moviesNo = moviesNo;
    data->actorsNo = actorsNo;
    data->moviesNames = moviesNames;
    data->actorsNames = actorsNames;

    fclose(inputFile);
    return data;
}

void destroy_input_data(inputData* data) {
    // unpacking data
    int moviesNo = data->moviesNo;
    int* actorsNo = data->actorsNo;
    char** moviesNames = data->moviesNames;
    char*** actorsNames = data->actorsNames; 

    for (int i = 0; i < moviesNo; i++) {
        free(moviesNames[i]);
        for (int j = 0; j < actorsNo[i]; j++) {
            free(actorsNames[i][j]);
        }
        free(actorsNames[i]);
    }
    free(actorsNames);
    free(moviesNames);
    free(actorsNo);
    free(data);
}

TGraphL* createGraphAdjList(int nodesNo) {
	TGraphL* graph = (TGraphL*) malloc(sizeof(TGraphL));
	graph->nn = nodesNo;

	graph->adl = (TNode**) malloc(nodesNo * sizeof(TNode*));
	for (int i = 0; i < nodesNo; i++) {
		graph->adl[i] = NULL;
	}
	return graph;
}

void addEdgeList(TGraphL* graph, int v1, int v2) {
    TNode* n1 = (TNode*) malloc(sizeof(TNode));
	TNode* n2 = (TNode*) malloc(sizeof(TNode));

	n1->v = v2;
	n1->next = graph->adl[v1];
	graph->adl[v1] = n1;

	n2->v = v1;
	n2->next = graph->adl[v2];
	graph->adl[v2] = n2;
}

TGraphL* createGraph(inputData* data) {
    // unpacking data
    int moviesNo = data->moviesNo;
    int* actorsNo = data->actorsNo;
    char*** actorsNames = data->actorsNames; 

    int totalActorsNo = 0; // including duplicates

    int** actorIndex = (int**) malloc(moviesNo * sizeof(int*));
    for (int i = 0; i < moviesNo; i++) {
        totalActorsNo += actorsNo[i];
        actorIndex[i] = (int*) calloc(actorsNo[i], sizeof(int));
    }

    char** nodeNames = (char**) malloc(totalActorsNo * sizeof(char*));
    int nodesNo = 0;
    int i, j, k;
    
    // indexing each actor
    for (i = 0; i < moviesNo; i++) {
        for (j = 0; j < actorsNo[i]; j++) {
            for (k = 0; k < nodesNo; k++) {
                if (strcmp(actorsNames[i][j], nodeNames[k]) == 0) {
                    actorIndex[i][j] = k;
                    break;
                }
            }
            // the actor hasn't been already indexed
            if (k == nodesNo) {
                actorIndex[i][j] = k;
                nodeNames[k] = strdup(actorsNames[i][j]);
                nodesNo++;
            }
        }
    }

    TGraphL* graph = createGraphAdjList(nodesNo);
    graph->nodeNames = nodeNames;
    TNode* it;

    // creating edges between actors
    for (i = 0; i < moviesNo; i++) {
        for (j = 0; j < actorsNo[i]; j++) {
            for (k = j + 1; k < actorsNo[i]; k++) {
                // checking if the edge already exists
                it = graph->adl[actorIndex[i][j]];
                while (it != NULL) {
                    if (it->v == actorIndex[i][k]) {
                        break;
                    }
                    it = it->next;
                }
                // adding edge
                if (it == NULL) {
                    addEdgeList(graph, actorIndex[i][j], actorIndex[i][k]);
                }
            }
        }
    }

    // freeing unnecessary data
    destroy_input_data(data);
    for (int i = 0; i < moviesNo; i++) {
        free(actorIndex[i]);
    }
    free(actorIndex); 

    return graph;
}

void destroyGraph(TGraphL* graph){
	for (int i = 0; i < graph->nn; i++) {
		TNode* it = graph->adl[i];
		while (it != NULL) {
			TNode* aux = it;
			it = it->next;
			free(aux);
		}
	}
    free(graph->adl);

    for (int i = 0; i < graph->nn; i++) {
        free(graph->nodeNames[i]);
    }
    free(graph->nodeNames);
	
	free(graph);
}

int main(int argc, char *argv[]) {
    if (argc == 4) {
        char* input_name = argv[2];
        char* output_name = argv[3];

        inputData* data = read_data(input_name);
        if (data == NULL) {
            printf("Fisierul de intrare nu a fost gasit.\n");
            return -1;
        }
        TGraphL* graph = createGraph(data);

        switch (argv[1][2]) {
            case '1':
                task1(graph, output_name);
                break;
            case '2':
                task2(graph, input_name, output_name);
                break;
            case '3':
                task3(graph, output_name);
                break;
            default:
                printf("Nu a fost specificata cerinta corect.\n");
                break;
        }

        destroyGraph(graph);

    } else {
        printf("Argumentele de intrare nu sunt suficiente.\n");
    }
    return 0;
}