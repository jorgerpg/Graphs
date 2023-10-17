#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // For INT_MAX
#include <stdbool.h>

typedef struct AdjListNode
{
  int dest;
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
  AdjList *array;
} Graph;

AdjListNode *newAdjListNode(int dest)
{
  AdjListNode *newNode = (AdjListNode *)malloc(sizeof(AdjListNode));
  newNode->dest = dest;
  newNode->next = NULL;
  return newNode;
}

void addEdge(Graph *graph, int src, int dest)
{
  AdjListNode *newNode = newAdjListNode(dest);
  newNode->next = graph->array[src].head;
  graph->array[src].head = newNode;
}

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
    if (line[i] != ' ' && line[i] != '\n' && line[i] != '\r')
    {
      ++V;
    }
  }

  char vertexLabels[V];
  for (int i = 0, j = 0; line[i] != '\0'; ++i)
  {
    if (line[i] != ' ' && line[i] != '\n' && line[i] != '\r')
    {
      vertexLabels[j] = line[i];
      ++j;
    }
  }

  // Allocate memory for the graph structure
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  graph->V = V;
  graph->labels = (char *)malloc(V * sizeof(char));
  // ... Initializing adjacency lists ...
  for (int i = 0; i < V; ++i)
  {
    graph->labels[i] = vertexLabels[i];
  }

  graph->array = (AdjList *)malloc(V * sizeof(AdjList));
  for (int i = 0; i < V; ++i)
  {
    graph->array[i].head = NULL;
  }

  for (int i = 0; i < V; ++i)
  {
    for (int j = 0; j < V; ++j)
    {
      int weight;
      fscanf(file, "%d", &weight);

      // If the weight is 1, add an edge between vertices i and j
      if (weight == 1)
      {
        addEdge(graph, i, j);
      }
    }
  }

  fclose(file);
  return graph;
}

// Function to print the adjacency list representation of the graph
void printGraph(Graph *graph)
{
  for (int v = 0; v < graph->V; ++v)
  {
    printf("Adjacencies of vertex %c: ", graph->labels[v]);
    AdjListNode *current = graph->array[v].head;
    while (current != NULL)
    {
      printf("%c -> ", graph->labels[current->dest]);
      current = current->next;
    }
    printf("NULL\n");
  }
}

int minDistance(int dist[], bool sptSet[], int V)
{
  int min = INT_MAX, min_index;

  for (int v = 0; v < V; v++)
  {
    if (!sptSet[v] && dist[v] < min)
    {
      min = dist[v];
      min_index = v;
    }
  }
  return min_index;
}

void printShortestPath(int dist[], int parent[], int V, char labels[])
{
  printf("Vertex   Distance from Source   Path\n");
  for (int i = 0; i < V; i++)
  {
    printf("%c -> %c \t\t %d \t\t", labels[i], labels[parent[i]], dist[i]);
    int j = i;
    while (j != -1)
    {
      printf("%c ", labels[j]);
      j = parent[j];
    }
    printf("\n");
  }
}

void dijkstra(Graph *graph, int src)
{
  int V = graph->V;
  char *labels = graph->labels;
  AdjList *array = graph->array;

  int dist[V];    // Array to store the shortest distance from src to i
  bool sptSet[V]; // Array to mark visited vertices
  int parent[V];  // Array to store the parent of each vertex in the shortest path

  for (int i = 0; i < V; i++)
  {
    dist[i] = INT_MAX;
    sptSet[i] = false;
    parent[i] = -1;
  }

  dist[src] = 0;

  for (int count = 0; count < V - 1; count++)
  {
    int u = minDistance(dist, sptSet, V);
    sptSet[u] = true;

    for (AdjListNode *v = array[u].head; v != NULL; v = v->next)
    {
      if (!sptSet[v->dest] && dist[u] != INT_MAX &&
          dist[u] + 1 < dist[v->dest])
      {
        dist[v->dest] = dist[u] + 1;
        parent[v->dest] = u;
      }
    }
  }

  printShortestPath(dist, parent, V, labels);
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: %s <file1>\n", argv[0]);
    return 1;
  }

  char *file = argv[1];
  Graph *graph = createGraph(file);

  printf("Graph:\n");
  printf("Vertex labels: %s\n", graph->labels);

  printGraph(graph);

  printf("\n\n");

  int sourceVertex = 0;
  dijkstra(graph, sourceVertex);

  // Free memory
  free(graph->labels);
  free(graph->array);
  free(graph);

  return 0;
}