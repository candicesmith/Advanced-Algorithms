#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <queue>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <cmath>

using namespace std;

/* QUESTION 5 - Ladder-gram */

int readFile(set<string>& dict, string& source) {
	string line;
	ifstream inFile("dictionary.txt");
	if (inFile.is_open()) {
		while (getline(inFile, line)) {
			if (line.size() == source.size())
				dict.insert(line);
		}
		inFile.close();
	}
	else {
		cout << "Unable to open file." << endl;
		return EXIT_FAILURE;
	}
}

vector<vector<string>> BFS(string source, string target, set<string> dict) {
    vector<vector<string>> allPaths;
    queue<vector<string>> paths;
    dict.insert(target);

    // A queue of paths is used to find all paths to target.
    paths.push({ source });

    // depth tracks the current node depth.
    int depth = 1;

    // depthLimit ensures the search does not
    // continue once the target word is found.
    int depthLimit = INT_MAX;
    
    // A visited words set is used to ensure 
    // no word is used twice in a path.
    set<string> visited;


    while (!paths.empty()) {
        vector<string> currPath = paths.front();
        paths.pop();


        if (currPath.size() > depth) {
            for (string s : visited) 
                dict.erase(s);

            visited.clear();

            if (currPath.size() > depthLimit)
                break;
            else
                depth = currPath.size();
        }

        // Grabs the last word in the current path
        // which will be altered next.
        string currWord = currPath.back();

        for (int i = 0; i < currWord.size(); ++i) {
            string nextWord = currWord;

            // Finds all valid adjecent words and inserts 
            // them into a temporary path called tempPath.
            for (char c = 'a'; c <= 'z'; ++c) {
                if (nextWord[i] == c)
                    continue;
                nextWord[i] = c;
                auto it = dict.find(nextWord);

                // If nextWord is in the dictionary it is
                // pushed to the tempPath and set as visited.
                if (it != dict.end()) {
                    vector<string> tempPath = currPath;
                    tempPath.push_back(nextWord);
                    visited.insert(nextWord);

                    // If the target word is found tempPath is
                    // pushed to the allPaths set and depthLimit
                    // is set to the current depth so it does 
                    // not search any deeper levels.
                    if (nextWord == target) {
                        depthLimit = depth;
                        allPaths.push_back(tempPath);
                    }
                    else
                        paths.push(tempPath);
                }
            }
        }
    }
    return allPaths;
}

void printShortestPath(vector<vector<string>>& path, string source, string target) {
    int shortestPathLen = path[0].size();
    vector<string> shortestPath = path[0];
    cout << "Path: ";
    for (int i = 1; i < path.size(); i++) {
        if (path[i].size() < shortestPathLen) {
            shortestPathLen = path[i].size();
            shortestPath = path[i];
        }
    }

    for (int i = 0; i < shortestPath.size(); i++) {
        cout << shortestPath[i] << " ";
    }
    cout << endl << endl;
    cout << source << " -> " << target << " in " << shortestPathLen - 1 << " steps" << endl << endl;
}

int main() {
	string source, target, input;
    set<string> dict; 
    vector<vector<string>> paths;
    int turn = 0;

	while (true) {
        cout << "Source word: " << endl;
        cin >> source;
        cout << "\nTarget word: " << endl;
        cin >> target;
        
		if (source.size() != target.size()) {
			cout << "\nSource word and target word must be the same length.\n" << endl;
			continue;
		}
		
        // This ensure the dictionary is only processed once.
        if (turn == 0) {
            // Creates a set of strings as the dictionary of words.
            cout << "Reading dictionary...\n";
            readFile(dict, source);
        }
        turn = 1;

        // Starts timer
		chrono::time_point<chrono::system_clock> start, end;
		start = chrono::system_clock::now();

        // Performs BFS and returns all paths from source to target.
		paths = BFS(source, target, dict);

        //Finds and prints the shortest path and its length.
        printShortestPath(paths, source, target);

        // Ends timer
		end = chrono::system_clock::now();
		double timeElapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
		cout << "Time elapsed: " << timeElapsed / 1000.00 << " sec" << endl;

		cout << endl;
		cout << "Enter 0 to exit or 1 to continue: " << endl;
		cin >> input;
		cout << endl;
		if (input == "0")
			return EXIT_SUCCESS;
	}
}