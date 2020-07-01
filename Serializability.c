/*
 * Serializability.c
 *
 *  Created on: 25-Apr-2020
 *      Author: GAURAV kumar
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

struct node {
    int vertex;
    struct node *next;
};

struct Graph {
    int numVertices;
    int *visited;
    struct node **adjLists; /* we need int** to store a two dimensional array. Similary,*/
                            /* we need struct node** to store an array of Linked lists*/
    struct node* (*createNode)(int v);
    bool (*isCyclic)(struct Graph *graph);
    void (*addEdge)(struct Graph *graph, int src, int dest);
    void (*printGraph)(struct Graph *graph);
};

bool isCyclicUtil(int v, bool visited[], bool *recStack, struct Graph *graph) {
    int i;
    struct node *adjList = graph->adjLists[v];
    struct node *temp = adjList;
    if (visited[v] == false) {
        /*Mark the current node as visited and part of recursion stack*/
        visited[v] = true;
        recStack[v] = true;

        /* Recur for all the vertices adjacent to this vertex*/
        while (temp != NULL) {
            int connectedVertex = temp->vertex;
            if (!visited[connectedVertex]
                    && isCyclicUtil(connectedVertex, visited, recStack, graph))
                return true;
            else if (recStack[connectedVertex])
                return true;
            temp = temp->next;
        }

    }
    recStack[v] = false; /* remove the vertex from recursion stack*/
    return false;
}

bool isCyclic(struct Graph *graph) {
    /* Mark all the vertices as not visited and not part of recursion*/
    /* stack*/
    int i;
    bool *visited = (bool*) malloc(graph->numVertices * sizeof(bool));
    bool *recStack = (bool*) malloc(graph->numVertices * sizeof(bool));
    for (i = 0; i < graph->numVertices; i++) {
        visited[i] = false;
        recStack[i] = false;
    }

    /* Call the recursive helper function isCyclicUtil(int v, bool visited[], bool *recStack, struct Graph *graph)*/
    /* to detect cycle in different*/
    /* DFS trees*/
    for (i = 0; i < graph->numVertices; i++)
        if (isCyclicUtil(i, visited, recStack, graph))
            return true;

    return false;
}

struct node* createNode(int v) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int no_of_vertices) {
    struct Graph *graph = malloc(sizeof(struct Graph));
    graph->numVertices = no_of_vertices;

    graph->adjLists = malloc(no_of_vertices * sizeof(struct node*));

    graph->visited = malloc(no_of_vertices * sizeof(int));

    int i;
    for (i = 0; i < no_of_vertices; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}

void addEdge(struct Graph *graph, int src, int dest) {
    /* Add edge from src to dest*/
    struct node *newNode = graph->createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
}

void printGraph(struct Graph *graph) {
    int v;
    for (v = 0; v < graph->numVertices; v++) {
        struct node *temp = graph->adjLists[v];
        printf("\n Adjacency list of vertices %d\n ", v);
        while (temp) {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}
/*this function makes the graph of transactions*/
void checkSerializability(char (*schedule)[4], int size_of_schedule,
        struct Graph *graph, bool *flag) {
    char temp1[4], temp2[4], temp[4];
    int i, index, src, dest;

    for (index = 0; index < size_of_schedule; index++) {
    
        strcpy(temp, schedule[index]);
        strcpy(temp1, schedule[index]);
        strcpy(temp2, schedule[index]);

        if (temp[0] == 'r' && temp[1] == '1') {
            temp1[0] = 'w';
            temp1[1] = '2';
        } else if (temp[0] == 'r' && temp[1] == '2') {
            temp1[0] = 'w';
            temp1[1] = '1';
        } else if (temp[0] == 'w' && temp[1] == '1') {
            temp1[0] = 'r';
            temp1[1] = '2';
            temp2[0] = 'w';
            temp2[1] = '2';
        } else if (temp[0] == 'w' && temp[1] == '2') {
            temp1[0] = 'r';
            temp1[1] = '1';
            temp2[0] = 'w';
            temp2[1] = '1';
        }
        if (temp[0] == 'r') {
            for (i = index + 1; i < size_of_schedule; i++) {

                if (strcmp(schedule[i], temp1) == 0) {
                    src = temp[1] - '0';
                    dest = temp1[1] - '0';

                    graph->addEdge(graph, src, dest);
                    *flag = true;
                }
            }
        } else {
            for (i = index + 1; i < size_of_schedule; i++) {
                if ((strcmp(schedule[i], temp1) == 0)
                        || (strcmp(schedule[i], temp2) == 0)) {
                    src = temp[1] - '0';
                    dest = temp1[1] - '0';
                    graph->addEdge(graph, src, dest);
                    *flag = true;

                }
            }
        }
    }
}

int main() {
    /*inputs*/
    char schedule[10][4] = { "r1x", "r2z", "r1z", "r1y", "r2y", "w1x", "w1y",
            "r2y", "w2z", "w2y" };
    /* char schedule[4][4] = { "r1x", "w2x", "r1y", "w2y" }; */
    struct Graph *graph = createGraph(4);
    graph->createNode = &createNode;
    graph->addEdge = &addEdge;
    graph->printGraph = &printGraph;
    graph->isCyclic = &isCyclic;
    int i;
    bool flag = false;
    checkSerializability(schedule, 10, graph, &flag); /*Here 10 is size of schedule*/
    //graph->printGraph(graph);
    if (!flag) {
        printf("Serial schedule.\n");
    } else if (graph->isCyclic(graph))
        printf("Not serialisable schedule.\n");
    else
        printf("Serialisable schedule.\n");
    return 0;
}
