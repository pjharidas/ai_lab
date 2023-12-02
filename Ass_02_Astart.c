#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

typedef struct GraphNode {
    int vertex;
    int cost;
    struct GraphNode* next;
} GraphNode;

typedef struct PriorityQueueNode {
    int vertex;
    int cost;
} PriorityQueueNode;

typedef struct PriorityQueue {
    int maxSize;
    int size;
    PriorityQueueNode* heap;
} PriorityQueue;

GraphNode* createNode(int vertex, int cost) {
    GraphNode* newNode = (GraphNode*)malloc(sizeof(GraphNode));
    newNode->vertex = vertex;
    newNode->cost = cost;
    newNode->next = NULL;
    return newNode;
}

PriorityQueueNode createPriorityQueueNode(int vertex, int cost) {
    PriorityQueueNode newNode;
    newNode.vertex = vertex;
    newNode.cost = cost;
    return newNode;
}

PriorityQueue* createPriorityQueue(int maxSize) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->maxSize = maxSize;
    pq->size = 0;
    pq->heap = (PriorityQueueNode*)malloc(sizeof(PriorityQueueNode) * (maxSize + 1));
    return pq;
}

void swap(PriorityQueueNode* a, PriorityQueueNode* b) {
    PriorityQueueNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(PriorityQueue* pq, int index) {
    while (index > 1) {
        int parentIndex = index / 2;
        if (pq->heap[index].cost < pq->heap[parentIndex].cost) {
            swap(&pq->heap[index], &pq->heap[parentIndex]);
            index = parentIndex;
        } else {
            break;
        }
    }
}

void insert(PriorityQueue* pq, int vertex, int cost) {
    if (pq->size >= pq->maxSize) {
        printf("Priority queue is full.\n");
        return;
    }
    pq->size++;
    pq->heap[pq->size] = createPriorityQueueNode(vertex, cost);
    heapifyUp(pq, pq->size);
}

void heapifyDown(PriorityQueue* pq, int index) {
    while (true) {
        int leftChild = 2 * index;
        int rightChild = 2 * index + 1;
        int smallest = index;

        if (leftChild <= pq->size && pq->heap[leftChild].cost < pq->heap[smallest].cost) {
            smallest = leftChild;
        }

        if (rightChild <= pq->size && pq->heap[rightChild].cost < pq->heap[smallest].cost) {
            smallest = rightChild;
        }

        if (smallest != index) {
            swap(&pq->heap[index], &pq->heap[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}

PriorityQueueNode deleteMin(PriorityQueue* pq) {
    PriorityQueueNode emptyNode;
    emptyNode.vertex = -1;
    emptyNode.cost = INT_MAX;

    if (pq->size == 0) {
        printf("Priority queue is empty.\n");
        return emptyNode;
    }

    PriorityQueueNode minNode = pq->heap[1];
    pq->heap[1] = pq->heap[pq->size];
    pq->size--;
    heapifyDown(pq, 1);
    return minNode;
}

void printPriorityQueue(PriorityQueue* pq) {
    printf("Priority Queue: ");
    for (int i = 1; i <= pq->size; i++) {
        printf("(%d, %d) ", pq->heap[i].vertex, pq->heap[i].cost);
    }
    printf("\n");
}

void printOpenCloseListsWithCost(bool visited[], int openList[], int openListSize, int closeList[], int closeListSize, int cost[], int heuristic[]) {
    printf("Open List: ");
    for (int i = 0; i < openListSize; i++) {
        int vertex = openList[i];
        int totalCost = cost[vertex] + heuristic[vertex];
        printf("(%d, Cost: %d) ", vertex, totalCost);
    }
    printf("\n");

    printf("Close List : ");
    for (int i = 0; i < closeListSize; i++) {
        int vertex = closeList[i];
        int totalCost = cost[vertex] + heuristic[vertex];
        printf("(%d, Cost: %d) ", vertex, totalCost);
    }
    printf("\n");
}

void aStarSearch(GraphNode* graph[], int numNodes, int startNode, int goalNode, int* path, int* pathLength, int* totalCost, int* heuristic) {
    PriorityQueue* pq = createPriorityQueue(numNodes);
    bool visited[numNodes];
    int parent[numNodes];
    int cost[numNodes];
    for (int i = 0; i < numNodes; i++) {
        visited[i] = false;
        parent[i] = -1;
        cost[i] = INT_MAX;
    }

    insert(pq, startNode, heuristic[startNode]);
    cost[startNode] = 0;

    int openList[numNodes];
    int closeList[numNodes];
    int openListSize = 0;
    int closeListSize = 0;

    while (pq->size > 0) {
        // Delete the minimum vertex from the priority queue
        PriorityQueueNode currentNode = deleteMin(pq);
        int currentVertex = currentNode.vertex;

        visited[currentVertex] = true;
        closeList[closeListSize++] = currentVertex;

        // Remove the node from the open list
        int openIndex = -1;
        for (int i = 0; i < openListSize; i++) {
            if (openList[i] == currentVertex) {
                openIndex = i;
                break;
            }
        }
        if (openIndex != -1) {
            openList[openIndex] = openList[openListSize - 1];
            openListSize--;
        }

        if (currentVertex == goalNode) {
            int current = goalNode;
            int index = 0;
            while (current != -1) {
                path[index++] = current;
                current = parent[current];
            }

            for (int i = 0, j = index - 1; i < j; i++, j--) {
                int temp = path[i];
                path[i] = path[j];
                path[j] = temp;
            }

            *pathLength = index;
            *totalCost = cost[goalNode];
            return;
        }

        GraphNode* neighbor = graph[currentVertex];
        while (neighbor != NULL) {
            int neighborVertex = neighbor->vertex;
            int neighborCost = neighbor->cost;

            if (!visited[neighborVertex]) {
                int newCost = cost[currentVertex] + neighborCost;

                if (newCost < cost[neighborVertex]) {
                    cost[neighborVertex] = newCost;
                    insert(pq, neighborVertex, newCost + heuristic[neighborVertex]);
                    parent[neighborVertex] = currentVertex;

                    // Add the neighbor to the open list if it's not already in it
                    int isOpen = 0;
                    for (int i = 0; i < openListSize; i++) {
                        if (openList[i] == neighborVertex) {
                            isOpen = 1;
                            break;
                        }
                    }
                    if (!isOpen) {
                        openList[openListSize++] = neighborVertex;
                    }
                }
            }
            neighbor = neighbor->next;
        }

        printOpenCloseListsWithCost(visited, openList, openListSize, closeList, closeListSize, cost, heuristic);
    }

    printf("Goal node %d not reached.\n");
}

int main() {
    int numNodes, numEdges;
    printf("Enter the number of nodes in the graph: ");
    scanf("%d", &numNodes);

    GraphNode* graph[numNodes];
    for (int i = 0; i < numNodes; i++) {
        graph[i] = NULL;
    }

    printf("Enter the number of edges in the graph: ");
    scanf("%d", &numEdges);

    printf("Enter the edges and costs (format: source destination cost):\n");
    for (int i = 0; i < numEdges; i++) {
        int source, destination, edgeCost;
        scanf("%d %d %d", &source, &destination, &edgeCost);

        GraphNode* newNode = createNode(destination, edgeCost);
        newNode->next = graph[source];
        graph[source] = newNode;
    }

    int startNode, goalNode;
    printf("Enter the start node: ");
    scanf("%d", &startNode);
    printf("Enter the goal node: ");
    scanf("%d", &goalNode);

    int path[numNodes];
    int pathLength = 0;
    int totalCost = 0;

    int heuristic[numNodes];
    printf("Enter heuristic values for each node:\n");
    for (int i = 0; i < numNodes; i++) {
        printf("Heuristic for node %d: ", i);
        scanf("%d", &heuristic[i]);
    }

    aStarSearch(graph, numNodes, startNode, goalNode, path, &pathLength, &totalCost, heuristic);

    if (pathLength > 0) {
        printf("Optimal Path from Node %d to Node %d (Cost: %d):\n", startNode, goalNode, totalCost);
        printf("Path: ");
        for (int i = 0; i < pathLength; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
    }

    return 0;
}
