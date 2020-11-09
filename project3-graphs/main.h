// packs all input data needed
typedef struct {
    int moviesNo;
    int* actorsNo;
    char** moviesNames;
    char*** actorsNames; 
} inputData;

typedef struct node {
	int v; // Destination node index
	struct node* next; // Link to the next node in the list
} TNode;

typedef struct{
	int nn; // Number of nodes in the graph
    char** nodeNames; // Actors' names
	TNode** adl; // Adjacency list 
} TGraphL;

typedef struct{
    char* actor1Name;
    char* actor2Name;
} TPair;

typedef struct{
    int u;
    int v;
} TBridge;

typedef struct{
    int size;
    TBridge* arr;
} TBridgeArray;

void task1(TGraphL* graph, char* output_name);
void task2(TGraphL* graph, char* input_name, char* output_name);
void task3(TGraphL* graph, char* output_name);
