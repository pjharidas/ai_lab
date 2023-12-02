#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

typedef struct GraphNode {
    int vertex;
    struct GraphNode* next;
} GraphNode;

typedef struct PriorityQueueNode {
    int vertex;
    int heuristic;
} PriorityQueueNode;

typedef struct PriorityQueue {
    int maxSize;
    int size;
    PriorityQueueNode* heap;
} PriorityQueue;

GraphNode* createNode(int vertex) {
    GraphNode* newNode = (GraphNode*)malloc(sizeof(GraphNode));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

PriorityQueueNode createPriorityQueueNode(int vertex, int heuristic) {
    PriorityQueueNode newNode;
    newNode.vertex = vertex;
    newNode.heuristic = heuristic;
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
        if (pq->heap[index].heuristic < pq->heap[parentIndex].heuristic) {
            swap(&pq->heap[index], &pq->heap[parentIndex]);
            index = parentIndex;
        } else {
            break;
        }
    }
}

void insert(PriorityQueue* pq, int vertex, int heuristic) {
    if (pq->size >= pq->maxSize) {
        printf("Priority queue is full.\n");
        return;
    }
    pq->size++;
    pq->heap[pq->size] = createPriorityQueueNode(vertex, heuristic);
    heapifyUp(pq, pq->size);
}

void heapifyDown(PriorityQueue* pq, int index) {
    while (true) {
        int leftChild = 2 * index;
        int rightChild = 2 * index + 1;
        int smallest = index;

        if (leftChild <= pq->size && pq->heap[leftChild].heuristic < pq->heap[smallest].heuristic) {
            smallest = leftChild;
        }

        if (rightChild <= pq->size && pq->heap[rightChild].heuristic < pq->heap[smallest].heuristic) {
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
    emptyNode.heuristic = INT_MAX;

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

void printOpenList(PriorityQueue* pq) {
    printf("Open List:\n");
    PriorityQueueNode* sortedHeap = (PriorityQueueNode*)malloc(sizeof(PriorityQueueNode) * (pq->maxSize + 1));
    int sortedSize = pq->size;
    for (int i = 1; i <= pq->size; i++) {
        sortedHeap[i] = pq->heap[i];
    }
    for (int i = 1; i < sortedSize; i++) {
        int minIndex = i;
        for (int j = i + 1; j <= sortedSize; j++) {
            if (sortedHeap[j].heuristic < sortedHeap[minIndex].heuristic) {
                minIndex = j;
            }
        }
        PriorityQueueNode temp = sortedHeap[i];
        sortedHeap[i] = sortedHeap[minIndex];
        sortedHeap[minIndex] = temp;
    }
    for (int i = 1; i <= sortedSize; i++) {
        printf("Node: %d, Heuristic: %d\n", sortedHeap[i].vertex, sortedHeap[i].heuristic);
    }
    free(sortedHeap);
    printf("\n");
}

void printCloseList(bool visited[], int numNodes) {
    printf("Closed List :\n");
    for (int i = 0; i < numNodes; i++) {
        if (visited[i]) {
            printf("Node %d\n", i);
        }
    }
    printf("\n");
}

void bestFirstSearch(GraphNode* graph[], int numNodes, int startNode, int goalNode, int* path, int* pathLength) {
    PriorityQueue* pq = createPriorityQueue(numNodes);
    bool visited[numNodes];
    int parent[numNodes];
    int heuristic[numNodes];

    for (int i = 0; i < numNodes; i++) {
        visited[i] = false;
        parent[i] = -1;
        printf("Enter heuristic for node %d: ", i);
        scanf("%d", &heuristic[i]);
    }

    insert(pq, startNode, heuristic[startNode]);

    while (pq->size > 0) {
        PriorityQueueNode currentNode = deleteMin(pq);
        int currentVertex = currentNode.vertex;

        visited[currentVertex] = true;

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
            return;
        }

        GraphNode* neighbor = graph[currentVertex];
        while (neighbor != NULL) {
            int neighborVertex = neighbor->vertex;
            if (!visited[neighborVertex]) {
                insert(pq, neighborVertex, heuristic[neighborVertex]);
                parent[neighborVertex] = currentVertex;
            }
            neighbor = neighbor->next;
        }
        printOpenList(pq);
        printCloseList(visited, numNodes);
    }

    printf("Goal node %d not reached.\n", goalNode);
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
    printf("Enter the edges (format: source destination):\n");
    for (int i = 0; i < numEdges; i++) {
        int source, destination;
        scanf("%d %d", &source, &destination);
        GraphNode* newNode = createNode(destination);
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

    bestFirstSearch(graph, numNodes, startNode, goalNode, path, &pathLength);

    if (pathLength > 0) {
        printf("Shortest Path from Node %d to Node %d:\n", startNode, goalNode);
        for (int i = 0; i < pathLength; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
    }

    return 0;
}
