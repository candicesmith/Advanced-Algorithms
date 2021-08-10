#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_set>

using namespace std;

/* QUESTION 3 - Data structure for storing very large graphs */
void readFile(vector<unordered_set<int>>& G) {

    ifstream inputFile;
    string fileName("graph.txt");
    inputFile.open(fileName);

    int numNodes;
    inputFile >> numNodes;
    int u, v;

    G.resize(numNodes);

    if (inputFile.is_open()) {
        while (!inputFile.eof()) {
            inputFile >> u;
            inputFile >> v;

            G.at(u).insert(v);
        }
    }
}


/* Driver code */
int main() {
    vector<unordered_set<int>> G;
    cout << "Reading graph data..." << endl << endl;
    readFile(G);
    int u;
    int v;
    int option;

    while (true) {
        cout << "Select option: " << endl;
        cout << "\t1. Check if 2 vertices are connected\n\t2. Produce a list of all vertices connected to a vertex\n\t3. Exit" << endl;
        cin >> option;
        if (option == 1) {
            cout << "Enter vertex one: ";
            cin >> u;
            cout << "\nEnter vertext two: ";
            cin >> v;

            unordered_set<int>::const_iterator found = G.at(u).find(v);

            if (found == G.at(u).end())
                cout << u << " is not connected to " << v << endl;
            else
                cout << u << " is connected to " << *found << endl;
        }
        else if (option == 2) {
            cout << "Enter vertex: ";
            cin >> u;
            //unordered_set<int>::const_iterator it = G.at(u).begin();
            cout << endl;
            for (auto it = G.at(u).begin(); it != G.at(u).end(); it++) {
                cout << *it << endl;
                it++;
            }
        }
        else if (option == 3) {
            return 0;
        }
        else {
            cout << "Unvalid input." << endl << endl;
        }
    }
}
