#include <iostream>
#include <vector>
#include <limits>
#include <stack>

using namespace std;

/* QUESTION 2 - BELLMAN-FORD ALGORITHM */

struct Edge {
    int u;
    int v;
    int w;
};

struct Graph {
    int numV;
    int numE;
    vector<Edge> E;
};

void createGraph(int numVertices, int numEdges, Graph *G) {
    G->numV = numVertices;
    G->numE = numEdges;

    /* Create vector of edges making up the graph */
    G->E = { {0, 1, 6}, {0, 2, 7}, {1, 2, 8}, {1, 3, 5}, {1, 4, -4}, 
           {2, 3, -3}, {2, 4, 9}, {3, 1, -2}, {4, 0, 2}, {4, 3, 7} };

}

void bellmanFord(int startNode, Graph *G) {

    /* Number of vertices */
    int V = G->numV;
    /* Number of edges */
    int E = G->numE;

    /* Create distance vector for each vertex from node 
       zero and initialise all distances to infinity */
    vector<int> dist(V, INT_MAX);

    /* Set distance of start node to zero */
    dist.at(startNode) = 0;

    /* Relax edges V - 1 times */
    for (int i = 1; i <= V - 1; i++) {
        /* Vector for storing parent node to generate shortest path */
        vector<int> prevNode(V, NULL);

        /* Reset distance vector */
        vector<int> dist(V, INT_MAX);
        dist.at(startNode) = 0;

        /* Relax each edge */
        for (int j = 0; j < E; j++) {
            int node = G->E[j].u;
            int nextNode = G->E[j].v;
            int weight = G->E[j].w;

            if (dist.at(node) != INT_MAX && dist.at(node) + weight < dist.at(nextNode)) {
                dist.at(nextNode) = dist.at(node) + weight;
                prevNode.at(nextNode) = node;
            }
        }

        cout << "Shortest path to node " << i <<  ":" << endl;
        int currentNode = i;

        /* Use stack to reverse paths */
        stack<int> S;
        S.push(currentNode);
        
        while(currentNode != startNode) {
            currentNode = prevNode.at(currentNode);
            S.push(currentNode);
        }

        while (!S.empty()) {
            cout << S.top() << " ";
            S.pop();
        }
        cout << endl;
        cout << endl;

        /* Clear parent node and distance vectors */
        prevNode.clear();
        dist.clear();
    }

    /* Check for any negative weighted paths */
    for (int i = 0; i < E; i++) {
        int node = G->E[i].u;
        int nextNode = G->E[i].v;
        int weight = G->E[i].w;

        if (dist.at(node) != INT_MAX && dist.at(node) + weight < dist.at(nextNode)) {
            cout << "Warning: Graph contains negative weighted path(s)." << endl;
            return;
        }
    }
}

/* Driver code */
int main() {
    int V = 5;
    int E = 10;
    Graph G;

    createGraph(V, E, &G);
    bellmanFord(0, &G);
}
