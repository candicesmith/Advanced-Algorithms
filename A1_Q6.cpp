#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_set>
#include <map>
#include <queue>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <cmath>

using namespace std;

/* QUESTION 6 */

int readFile(vector<string>& dict, int wordLen) {
    string line;
    ifstream inFile("dictionary.txt");
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            if (line.size() == wordLen)
                dict.push_back(line);
        }
        inFile.close();
    }
    else {
        cout << "Unable to open file." << endl;
        return EXIT_FAILURE;
    }
}

void sortDict(vector<string>& dict, map<pair<char, char>, vector<string>>& sortedDict, int p1, int p2) {
    pair<char, char> key;

    for (int i = 0; i < dict.size(); i++) {
        key = make_pair(dict[i][p1], dict[i][p2]);
        auto fk = sortedDict.find(key);
        if (fk == sortedDict.end()) {
            sortedDict.insert(pair<pair<char, char>, vector<string>>(key, { dict[i] }));
        }
        else {
            sortedDict.at(key).push_back(dict[i]);
        }
    }
}

string findStartWord(map<pair<char, char>, vector<string>> sortedDict) {
    int maxLen = 0;
    pair<char, char> key;

    for (auto it = sortedDict.begin(); it != sortedDict.end(); it++) {
        if (it->second.size() > maxLen) {
            maxLen = it->second.size();
            key = it->first;
        }
    }

    return sortedDict[key].back();
}

string findNextWord(pair<char, char> endChars, map<pair<char, char>, vector<string >>& sortedDict, int wordLen, string currWord) {
    int p1 = wordLen - 2;
    int p2 = wordLen - 3;
    int wordMax = 0;
    int num = 0;
    string evalWord, nextWord;
    pair<char, char> evalChars;

    for (int i = 0; i < sortedDict.at(endChars).size()-1; i++) {
        evalWord = sortedDict.at(endChars).at(i);
        evalChars = make_pair(evalWord[p1], evalWord[p2]);
        auto fk = sortedDict.find(evalChars);
        if (evalWord.compare(currWord) != 0)
            continue;
        if (fk != sortedDict.end() && sortedDict.at(evalChars).size() >= wordMax) {
            wordMax = sortedDict.at(evalChars).size();
            nextWord.assign(evalWord);
        }
    }
    if (nextWord.empty()) {
        nextWord = sortedDict.at(endChars).back();
        sortedDict.at(endChars).pop_back();
    }
    
    return nextWord;
}

vector<string> linkedWords(map<pair<char, char>, vector<string>>& sortedDict, string currWord, int wordLen) {
    string nextWord;
    vector<string> currSeq;
    vector<string> maxLenSeq;
    pair<char, char> endChars;
    int p1 = wordLen - 3;
    int p2 = wordLen - 2;

    while (true) {
        endChars = make_pair(currWord.at(p1), currWord.at(p2));
        auto find = sortedDict.find(endChars);
        if (find == sortedDict.end()) {
            if (currSeq.size() > 1) {
                // backtracking
                currWord = currSeq.back();
                currSeq.pop_back();
                endChars = make_pair(currWord.at(p1), currWord.at(p2));
            }
            else
                break;
        }

        while (sortedDict.at(endChars).size() == 0) {
            if (currSeq.size() > 1) {
                if (currSeq.size() > maxLenSeq.size())
                    maxLenSeq = currSeq;
                currWord = currSeq.back();
                currSeq.pop_back();
                endChars = make_pair(currWord.at(p1), currWord.at(p2));
            }
            else
                break;
        }

        find = sortedDict.find(endChars);
        if (find != sortedDict.end() && sortedDict.at(endChars).size() != 0) {
            nextWord = findNextWord(endChars, sortedDict, wordLen, currWord);
            currSeq.push_back(currWord);
            currWord = nextWord;
        }
        else
            break;
    }

    return maxLenSeq;
}

int main() {
    while (true) {
        int wordLen;
        string source, target, input, startWord, in;
        vector<string> dict, maxLenSeq;
        vector<vector<string>> paths;
        map<pair<char, char>, vector<string>> sortedDict1, sortedDict2;

        cout << "Enter word length: ";
        cin >> wordLen;
        cout << "Reading dictionary...\n";
        readFile(dict, wordLen);
        cout << "Word Length: " << wordLen << endl;
        // Starts timer
        cout << "Num. Words: " << dict.size() << endl;
        chrono::time_point<chrono::system_clock> start, end, start1, end1;
        start = chrono::system_clock::now();

        sortDict(dict, sortedDict1, 1, 2);
        sortDict(dict, sortedDict2, wordLen - 3, wordLen - 2);

        start1 = chrono::system_clock::now();

        startWord = findStartWord(sortedDict2);
        maxLenSeq = linkedWords(sortedDict1, startWord, wordLen);

        end1 = chrono::system_clock::now();

        cout << "Seq. Length: " << maxLenSeq.size() << endl;

        // Ends timer
        end = chrono::system_clock::now();

        double timeElapsed1 = chrono::duration_cast<chrono::milliseconds>(end1 - start1).count();
        cout << "CPU Found: " << timeElapsed1 / 1000.00 << " sec" << endl;

        double timeElapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        cout << "CPU Total: " << timeElapsed / 1000.00 << " sec" << endl;
        cout << endl;

        cout << "Do you want to print the sequence? (y/n): ";
        cin >> in;
        if (in == "y") {
            for (int i = 0; i < maxLenSeq.size(); i++) {
                cout << maxLenSeq[i] << " ";
            }
            cout << endl << endl;
        }

        cout << "\nEnter [0] to exit or [1] to continue: " << endl;
        cin >> input;
        cout << endl;
        if (input == "0")
            return 0;

    }
}