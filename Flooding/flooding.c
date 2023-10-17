/**
 * @file flooding.c
 * @author Jorge Ricarte (jorgericartepg@gmail.com)
 * @brief A program to see if the graphs have connected components.
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
    bool* visited; // Novo campo para controle de flooding
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
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
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
    graph->visited = (bool*)malloc(V * sizeof(bool)); // Novo campo para controle de flooding
    // ... Initializing adjacency lists and visited array ...
    for (int i = 0; i < V; ++i) {
        graph->labels[i] = vertexLabels[i];
        graph->visited[i] = false; // Inicializa todos os vértices como não visitados
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

// Função para marcar vértices visitados usando flooding
void flood(Graph* graph, int v, bool visited[]) {
    visited[v] = true;
    AdjListNode* current = graph->array[v].head;
    while (current != NULL) {
        int dest = current->dest;
        if (!visited[dest]) {
            flood(graph, dest, visited);
        }
        current = current->next;
    }
}

// Função para contar o número de componentes conexas
int countConnectedComponents(Graph* graph) {
    int components = 0;
    bool* visited = (bool*)malloc(graph->V * sizeof(bool)); // Array temporário para rastrear visitas

    for (int i = 0; i < graph->V; ++i) {
        visited[i] = false;
    }

    for (int i = 0; i < graph->V; ++i) {
        if (!visited[i] && graph->array[i].head != NULL) {
            components++;
            flood(graph, i, visited);
        }
    }

    free(visited); // Liberar a memória alocada para o array temporário
    return components;
}



int main(int argc, char* argv[]) {
    // Verifique se há dois argumentos de linha de comando (dois arquivos de grafo)
    if (argc != 3) {
        printf("Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }

    char* file1 = argv[1];
    char* file2 = argv[2];

    // Crie os grafos
    Graph* graph1 = createGraph(file1);
    Graph* graph2 = createGraph(file2);

    printf("Graph 1:\n");
    printf("Vertex labels: %s\n", graph1->labels);

    printf("\nGraph 2:\n");
    printf("Vertex labels: %s\n", graph2->labels);

    int components1 = countConnectedComponents(graph1);
    int components2 = countConnectedComponents(graph2);

    printf("\nNumber of connected components in Graph 1: %d\n", components1);
    printf("Number of connected components in Graph 2: %d\n", components2);

    // Libere a memória alocada para os grafos, etiquetas e array visited
    free(graph1->labels);
    free(graph1->visited);
    free(graph1->array);
    free(graph1);

    free(graph2->labels);
    free(graph2->visited);
    free(graph2->array);
    free(graph2);

    return 0;
}
