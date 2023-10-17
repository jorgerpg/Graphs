/**
 * @file areIso.c
 * @author Jorge Ricarte (jorgericartepg@gmail.com)
 * @brief A program to check if two graphs are isomorphs.
 * @version 0.1
 * @date 2023-10-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Define structures for the graph representation
typedef struct AdjListNode {
    int dest;
    struct AdjListNode* next;
} AdjListNode;

typedef struct AdjList {
    AdjListNode* head;
} AdjList;

typedef struct Graph {
    int V;
    char* labels;
    AdjList* array;
} Graph;

// Function to create a new adjacency list node
AdjListNode* newAdjListNode(int dest) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

// Function to add an edge to the graph
void addEdge(Graph* graph, int src, int dest) {
    AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}

// Function to create a graph from a file
Graph* createGraph(char* fileName) {
    // Open the input file
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
    }

    
    // Read the first line to determine the number of vertices
    char line[100];
    fgets(line, sizeof(line), file);

    int V = 0;
    for (int i = 0; line[i] != '\0'; ++i) {
        if (line[i] != ' ' && line[i] != '\n') {
            ++V;
        }
    }

    char vertexLabels[V];
    for (int i = 0, j = 0; line[i] != '\0'; ++i) {
        if (line[i] != ' ' && line[i] != '\n') {
            vertexLabels[j] = line[i];
            ++j;
        }
    }

    // Allocate memory for the graph structure
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->labels = (char*)malloc(V * sizeof(char));
    // ... Initializing adjacency lists ...
    for (int i = 0; i < V; ++i) {
        graph->labels[i] = vertexLabels[i];
    }

    graph->array = (AdjList*)malloc(V * sizeof(AdjList));
    for (int i = 0; i < V; ++i) {
        graph->array[i].head = NULL;
    }

    // Populate the adjacency matrix from the file
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            int weight;
            fscanf(file, "%d", &weight);
            if (weight == 1) {
                addEdge(graph, i, j);
            }
        }
    }

    fclose(file);

    return graph;
}

// Function to print the adjacency list representation of the graph
void printGraph(Graph* graph) {
    for (int v = 0; v < graph->V; ++v) {
        printf("Adjacencies of vertex %c: ", graph->labels[v]);
        AdjListNode* current = graph->array[v].head;
        while (current != NULL) {
            printf("%c -> ", graph->labels[current->dest]);
            current = current->next;
        }
        printf("NULL\n");
    }
}

// Function to check if two vertices are connected by an edge
bool areConnected(Graph *graph, int vertex1, int vertex2) {
    AdjListNode *current = graph->array[vertex1].head;
    while (current != NULL) {
        if (current->dest == vertex2) {
            return true;  // Vertex2 is in the adjacency list of Vertex1
        }
        current = current->next;
    }
    return false;  // No edge between Vertex1 and Vertex2
}

// Function to check if a vertex mapping is isomorphic
bool isMappingIsomorphic(Graph *graph1, Graph *graph2, int mapping[]) {
    for (int i = 0; i < graph1->V; ++i) {
        for (int j = i + 1; j < graph1->V; ++j) {
            if (mapping[i] == mapping[j]) {
                return false;  // Vertex mapping must be one-to-one
            }

            if (areConnected(graph1, i, j) != areConnected(graph2, mapping[i], mapping[j])) {
                return false;  // Adjacency relationship must be preserved
            }
        }
    }
    return true;
}

// Function to find isomorphism using brute force
void findIsomorphism(Graph *graph1, Graph *graph2, int mapping[], int currentVertex) {
    if (currentVertex == graph1->V) {
        if (isMappingIsomorphic(graph1, graph2, mapping)) {
            printf("Isomorphic mapping found:\n");
            for (int i = 0; i < graph1->V; ++i) {
                printf("%c -> %c\n", graph1->labels[i], graph2->labels[mapping[i]]);
            }
            exit(0); // Exit after finding one isomorphic mapping
        }
        return;
    }

    for (int i = 0; i < graph2->V; ++i) {
        mapping[currentVertex] = i;
        findIsomorphism(graph1, graph2, mapping, currentVertex + 1);
    }
}

int main(int argc, char *argv[]) {
    // Get file names for the graphs
    if (argc != 3) {
        printf("Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }

    char *file1 = argv[1];
    char *file2 = argv[2];

    // Create graphs
    Graph *graph1 = createGraph(file1);
    Graph *graph2 = createGraph(file2);

    printf("Graph 1:\n");
    printf("Vertex labels: %s\n", graph1->labels);
    printGraph(graph1);

    printf("\nGraph 2:\n");
    printf("Vertex labels: %s\n", graph2->labels);
    printGraph(graph2);
    printf("\n");

    int mapping[graph1->V]; // Stores the vertex mapping

    findIsomorphism(graph1, graph2, mapping, 0);
    printf("No isomorphic mapping found.\n");

    // Free allocated memory for the graphs and labels
    free(graph1->labels);
    free(graph1->array);
    free(graph1);

    free(graph2->labels);
    free(graph2->array);
    free(graph2);

    return 0;
}
