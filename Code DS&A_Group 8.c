#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 26  // So luong toi da cac nut
#define MAX_EDGES 100 // So luong toi da cac canh

typedef struct Node {
    char name;
    int weight;
    int num_edges;
    struct Node* edges[MAX_EDGES];
    int edge_weights[MAX_EDGES];
} Node;

typedef struct Graph {
    int num_nodes;
    Node* nodes[MAX_NODES];
} Graph;

typedef struct QueueNode {
    Node* node;
    struct QueueNode* next; //Linked list FIFO (First-In-First-Out)
} QueueNode;

typedef struct Queue {
    QueueNode* front; //Queue phia truoc	
    QueueNode* rear; //Queue phia sau
} Queue;

// Tao mot nut moi voi ten va trong so cho truoc
Node* createNode(char name, int weight) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->name = name;
    newNode->weight = weight;
    newNode->num_edges = 0;
    return newNode;
}

// Tao mot do thi moi 
Graph* createGraph() {
    Graph* newGraph = (Graph*)malloc(sizeof(Graph));
    newGraph->num_nodes = 0;
    return newGraph;
}

// Them mot canh giua hai nut voi trong so cho truoc
void addEdge(Node* node, Node* edge, int weight) {
    if (node->num_edges < MAX_EDGES && edge->num_edges < MAX_EDGES) {
        node->edges[node->num_edges] = edge;
        node->edge_weights[node->num_edges] = weight;
        node->num_edges++;

        edge->edges[edge->num_edges] = node;
        edge->edge_weights[edge->num_edges] = weight;
        edge->num_edges++;
    }
}

// Them mot nut vao hang doi QUEUE
void enqueue(Queue* queue, Node* node) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->node = node;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Lay nut dau tien ra khoi hang doi
Node* dequeue(Queue* queue) {
    if (queue->front == NULL) {
        return NULL;
    } else {
        QueueNode* temp = queue->front;
        Node* node = temp->node;
        queue->front = queue->front->next;

        if (queue->front == NULL) {
            queue->rear = NULL;
        }

        free(temp);
        return node;
    }
}

// Tinh toan MSE cua mot nut
int calculateMSE(Node* node) {
    int mse = 0;
    int i;
    for (i = 0; i < node->num_edges; i++) {
        mse += node->weight * node->edge_weights[i];
    }
    return mse;
}

// In ra thong tin cua mot nut va cac lien ket cua no
void printGraph(Node* node, int nodeNum) {
    printf("Graph %d:\n%c\n", nodeNum, node->name);
}

// In ra thong tin cua cac nut (graph con con lai)
void printGraphRemain(Node* node, int nodeNum) {
    printf("%c ", node->name);
}

// Tim va in ra cac do thi con thoa man MSE > Threshold
void findConnectedGraphs(Graph* graph, int threshold) {
    int visited[MAX_NODES] = {0};                            // Mang danh dau nut da tham (ban dau tat ca deu chua tham)
    int graphCount = 0;                                      // Bien dem so luong do thi con thoa man dieu kien
	int i, j;
    Graph* remainingGraph = createGraph(); // Create a new graph to store nodes with MSE < Threshold

    for (i = 0; i < graph->num_nodes; i++) { 
        Node* node = graph->nodes[i];                        // Duyet qua tat ca cac nut trong do thi

        if (!visited[node->name - 'A']) {                    // Kiem tra xem nut co da duoc duyet qua chua
            Queue* queue = (Queue*)malloc(sizeof(Queue));    // Cap phat bo nho dong cho hang doi
            queue->front = NULL; 
            queue->rear = NULL;

            enqueue(queue, node);                            // Dua nut vao hang doi de bat dau duyet BFS tu nut nay
            visited[node->name - 'A'] = 1;                   // Danh dau nut hien tai la da duyet

            while (queue->front != NULL) {
                Node* currNode = dequeue(queue);             // Lay nut dau tien ra khoi hang doi de duyet

                int mse = calculateMSE(currNode);            

                if (mse >= threshold) {                      
                    graphCount++;                            // Tang bien dem so luong do thi thoa man
                    printGraph(currNode, graphCount);        // In thong tin do thi con
                } else {
                    // Add the node to the remainingGraph since it doesn't meet the threshold
                    remainingGraph->nodes[remainingGraph->num_nodes++] = currNode;
                }

                for (j = 0; j < currNode->num_edges; j++) { // Duyet qua tat ca cac nut neighbour cua nut hien tai
                    Node* nextNode = currNode->edges[j];

                    if (!visited[nextNode->name - 'A']) {       // Neu nut neighbour chua duoc duyet qua
                        enqueue(queue, nextNode);               // Dua nut neighbor vao hang doi de duyet sau
                        visited[nextNode->name - 'A'] = 1;      // Danh dau nut neighbour la da duyet
                    }
                }
            }

            free(queue); // Giai phong bo nho
        }
    }

    printf("\nNumbers of nodes that have MSE > Threshold: %d\n", graphCount);

    // In ra Graph con lai (MSE < Threshold)
    printf("Remaining Graph (MSE < Threshold) and still connected:\n"); 
    for (i = 0; i < remainingGraph->num_nodes; i++) {
        Node* node = remainingGraph->nodes[i];
        printGraphRemain(node, i + 1);
    }
}


int main() {
    Graph* graph = createGraph();

//Test case 1
    
    // Them cac nut vao do thi
    Node* nodeA = createNode('A', 3);
    Node* nodeB = createNode('B', 2);
    Node* nodeC = createNode('C', 4);
    Node* nodeD = createNode('D', 4);
    Node* nodeE = createNode('E', 5);
    Node* nodeF = createNode('F', 7);
    Node* nodeG = createNode('G', 6);
    Node* nodeH = createNode('H', 3);

    // Them cac lien ket
    addEdge(nodeA, nodeB, 2);
    addEdge(nodeA, nodeC, 6);
    addEdge(nodeA, nodeD, 4);
    addEdge(nodeA, nodeF, 2);
    addEdge(nodeB, nodeC, 3);
    addEdge(nodeB, nodeE, 2);
    addEdge(nodeC, nodeE, 7);
    addEdge(nodeD, nodeE, 3);
    addEdge(nodeE, nodeF, 1);
    addEdge(nodeF, nodeG, 3);
    addEdge(nodeF, nodeH, 4);


    graph->nodes[graph->num_nodes++] = nodeA;
    graph->nodes[graph->num_nodes++] = nodeB;
    graph->nodes[graph->num_nodes++] = nodeC;
    graph->nodes[graph->num_nodes++] = nodeD;
    graph->nodes[graph->num_nodes++] = nodeE;
    graph->nodes[graph->num_nodes++] = nodeF;
    graph->nodes[graph->num_nodes++] = nodeG;
    graph->nodes[graph->num_nodes++] = nodeH;

    int threshold;
    printf("Enter Threshold: ");
    scanf("%d", &threshold);

    findConnectedGraphs(graph, threshold);

    return 0;
}
