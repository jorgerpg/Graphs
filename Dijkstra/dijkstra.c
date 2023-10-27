/**
 * @file graphs-adjacency.c
 * @author Jorge Ricarte (jorgericartepg@gmail.com)
 * @brief A program for creating and printing the adjacency list representation of a graph.
 * @version 0.1
 * @date 2023-10-16
 * @copyright Copyright (c) 2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INT_MAX 9999

typedef struct AdjListNode
{
    int dest;
    int weight; // Added weight
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

AdjListNode *newAdjListNode(int dest, int weight) // Added weight parameter
{
    AdjListNode *newNode = (AdjListNode *)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight; // Set the weight
    newNode->next = NULL;
    return newNode;
}

void addEdge(Graph *graph, int src, int dest, int weight) // Added weight parameter
{
    AdjListNode *newNode = newAdjListNode(dest, weight);
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

            // If the weight is not 0, add an edge between vertices i and j
            if (weight != 0)
            {
                addEdge(graph, i, j, weight); // Added weight parameter
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
            printf("%c (%d) -> ", graph->labels[current->dest], current->weight); // Print the weight
            current = current->next;
        }
        printf("NULL\n");
    }
}

// Function to find the minimum distance vertex not yet included in the shortest path tree
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

// Function to implement Dijkstra's algorithm
void dijkstra(Graph *graph, int src)
{
    int V = graph->V;
    int dist[V];
    bool sptSet[V];

    for (int i = 0; i < V; i++)
    {
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++)
    {
        int u = minDistance(dist, sptSet, V);
        sptSet[u] = true;

        AdjListNode *current = graph->array[u].head;
        while (current != NULL)
        {
            int v = current->dest;
            if (!sptSet[v] && dist[u] != INT_MAX && dist[u] + current->weight < dist[v])
            {
                dist[v] = dist[u] + current->weight;
            }
            current = current->next;
        }
    }

    printf("Shortest distances from vertex %c:\n", graph->labels[src]);
    for (int i = 0; i < V; i++)
    {
        printf("To %c: %d\n", graph->labels[i], dist[i]);
    }
}


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <file1> <source_vertex>\n", argv[0]);
        return 1;
    }

    char *file = argv[1];
    Graph *graph = createGraph(file);
    int source_vertex = atoi(argv[2]); // Get the source vertex from the command line argument

    printf("Graph:\n");
    printf("Vertex labels: %s\n", graph->labels);

    printGraph(graph);

    dijkstra(graph, source_vertex);

    // Free memory
    free(graph->labels);
    free(graph->array);
    free(graph);

    return 0;
}