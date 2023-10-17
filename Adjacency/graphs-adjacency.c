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

// Define a structure to represent a node in the adjacency list
typedef struct AdjListNode
{
	int dest;									// Destination vertex
	struct AdjListNode *next; // Pointer to the next adjacent node
} AdjListNode;

// Define a structure to represent an adjacency list
typedef struct AdjList
{
	AdjListNode *head; // Pointer to the first adjacent node in the list
} AdjList;

// Define a structure to represent a graph
typedef struct Graph
{
	int V;					// Number of vertices in the graph
	char *labels;		// Labels for vertices
	AdjList *array; // An array of adjacency lists
} Graph;

// Function to create a new adjacency list node
AdjListNode *newAdjListNode(int dest)
{
	AdjListNode *newNode = (AdjListNode *)malloc(sizeof(AdjListNode));
	newNode->dest = dest;
	newNode->next = NULL;
	return newNode;
}

// Function to create a new graph
Graph *createGraph(int V, char *labels)
{
	Graph *graph = (Graph *)malloc(sizeof(Graph));
	graph->V = V;
	graph->labels = labels;
	graph->array = (AdjList *)malloc(V * sizeof(AdjList));
	for (int i = 0; i < V; ++i)
	{
		graph->array[i].head = NULL;
	}
	return graph;
}

// Function to add an edge between two vertices
void addEdge(Graph *graph, int src, int dest)
{
	AdjListNode *newNode = newAdjListNode(dest);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;
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

int main()
{
	// Open a file named "Adjacency.txt" for reading
	FILE *file = fopen("Adjacency.txt", "r");
	if (file == NULL)
	{
		printf("Error opening the file.\n");
		return 1;
	}

	char line[100];
	fgets(line, sizeof(line), file); // Read the first line of the file
	int V = 0;

	// Count the number of vertices based on the first line
	for (int i = 0; line[i] != '\0'; ++i)
	{
		if (line[i] != ' ' && line[i] != '\n')
		{
			++V;
		}
	}

	char vertexLabels[V];

	// Extract vertex labels from the first line
	for (int i = 0, j = 0; line[i] != '\0'; ++i)
	{
		if (line[i] != ' ' && line[i] != '\n')
		{
			vertexLabels[j] = line[i];
			++j;
		}
	}

	// Create a new graph with the specified number of vertices and labels
	Graph *graph = createGraph(V, vertexLabels);

	// Read the adjacency matrix from the file and add edges to the graph
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

	// Close the file
	fclose(file);

	// Print the adjacency list of the graph
	printf("Adjacency list of the graph:\n");
	printGraph(graph);

	return 0;
}