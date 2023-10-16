/**
 * @file prim.c
 * @author Jorge Ricarte (jorgericartepg@gmail.com)
 * @brief 
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
#include <limits.h>

// Define structures for the graph representation
typedef struct AdjListNode
{
  int dest;
  int weight;
  struct AdjListNode *next;
} AdjListNode;

typedef struct AdjList
{
  AdjListNode *head;
} AdjList;

typedef struct Graph
{
  int V;
  char *labels;
  bool *inMST; // Novo campo para controle de árvore geradora mínima
  AdjList *array;
} Graph;

// Function to create a new adjacency list node
AdjListNode *newAdjListNode(int dest, int weight)
{
  AdjListNode *newNode = (AdjListNode *)malloc(sizeof(AdjListNode));
  newNode->dest = dest;
  newNode->weight = weight;
  newNode->next = NULL;
  return newNode;
}

// Function to add an edge to the graph
void addEdge(Graph *graph, int src, int dest, int weight)
{
  AdjListNode *newNode = newAdjListNode(dest, weight);
  newNode->next = graph->array[src].head;
  graph->array[src].head = newNode;
}

// Function to create a graph from a file
Graph *createGraph(char *fileName)
{
  // Open the input file
  FILE *file = fopen(fileName, "r");
  if (file == NULL)
  {
    printf("Erro ao abrir o arquivo.\n");
    exit(1);
  }

  // Read the first line to determine the number of vertices
  char line[100];
  fgets(line, sizeof(line), file);

  int V = 0;
  for (int i = 0; line[i] != '\0'; ++i)
  {
    if (line[i] != ' ' && line[i] != '\n')
    {
      ++V;
    }
  }

  char vertexLabels[V];
  for (int i = 0, j = 0; line[i] != '\0'; ++i)
  {
    if (line[i] != ' ' && line[i] != '\n')
    {
      vertexLabels[j] = line[i];
      ++j;
    }
  }

  // Allocate memory for the graph structure
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  graph->V = V;
  graph->labels = (char *)malloc(V * sizeof(char));
  graph->inMST = (bool *)malloc(V * sizeof(bool)); // Novo campo para controle de árvore geradora mínima

  for (int i = 0; i < V; ++i)
  {
    graph->labels[i] = vertexLabels[i];
    graph->inMST[i] = false; // Inicializa todos os vértices como fora da árvore geradora mínima
  }

  graph->array = (AdjList *)malloc(V * sizeof(AdjList));
  for (int i = 0; i < V; ++i)
  {
    graph->array[i].head = NULL;
  }

  // Populate the adjacency matrix from the file
  for (int i = 0; i < V; ++i)
  {
    for (int j = 0; j < V; ++j)
    {
      int weight;
      fscanf(file, "%d", &weight);
      if (weight > 0)
      {
        addEdge(graph, i, j, weight);
      }
    }
  }

  fclose(file);

  return graph;
}

// Function to find the minimum key value vertex not yet included in MST
int minKey(int key[], bool inMST[], int V)
{
  int min = INT_MAX, min_index;

  for (int v = 0; v < V; v++)
  {
    if (!inMST[v] && key[v] < min)
    {
      min = key[v];
      min_index = v;
    }
  }

  return min_index;
}

// Function to print the MST
void printMST(int parent[], int graphSize, int **graph)
{
  printf("Edge \tWeight\n");
  for (int i = 1; i < graphSize; i++)
  {
    printf("%c - %c \t%d\n", graph[parent[i]][i], graph[i][parent[i]], graph[i][parent[i]]);
  }
}

// Function to find MST using Prim's algorithm
void primMST(Graph *graph)
{
  int V = graph->V;
  int parent[V];
  int key[V];

  for (int i = 0; i < V; i++)
  {
    key[i] = INT_MAX;
    parent[i] = -1;
  }

  key[0] = 0;

  for (int count = 0; count < V - 1; count++)
  {
    int u = minKey(key, graph->inMST, V);
    graph->inMST[u] = true;

    for (AdjListNode *v = graph->array[u].head; v; v = v->next)
    {
      if (!graph->inMST[v->dest] && v->weight < key[v->dest])
      {
        parent[v->dest] = u;
        key[v->dest] = v->weight;
      }
    }
  }

  // Print the MST
  printf("Minimum Spanning Tree (MST) found by Prim's algorithm:\n");
  for (int i = 1; i < V; i++)
  {
    printf("Edge: %c - %c, Weight: %d\n", graph->labels[parent[i]], graph->labels[i], key[i]);
  }
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s <file1>\n", argv[0]);
    return 1;
  }

  char *file1 = argv[1];
  Graph *graph1 = createGraph(file1);

  printf("Graph 1:\n");
  printf("Vertex labels: %s\n", graph1->labels);

  primMST(graph1);

  // Free memory
  free(graph1->labels);
  free(graph1->inMST);
  free(graph1->array);
  free(graph1);

  return 0;
}