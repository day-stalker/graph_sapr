#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <limits>
using namespace std;

const int MAX_NODES = 20;  // Maximum number of nodes
int distanceMatrix[MAX_NODES][MAX_NODES]; // Adjacency matrix
int nodeCount;             // Actual number of nodes

int optimalPath[MAX_NODES+1];
bool visitedNodes[MAX_NODES];
int minTourCost = INT_MAX;

// Function to read matrix from file
bool readDistanceMatrix(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file!" << endl;
        return false;
    }

    inputFile >> nodeCount;
    for (int i = 0; i < nodeCount; ++i) {
        for (int j = 0; j < nodeCount; ++j) {
            inputFile >> distanceMatrix[i][j];
        }
    }
    return true;
}

void saveOptimalPath(int currentPath[]) {
    for (int i = 0; i < nodeCount; i++)
        optimalPath[i] = currentPath[i];
    optimalPath[nodeCount] = currentPath[0];
}

int calculateFirstMin(int node) {
    int minValue = INT_MAX;
    for (int k = 0; k < nodeCount; k++)
        if (distanceMatrix[node][k] < minValue && node != k)
            minValue = distanceMatrix[node][k];
    return minValue;
}

int calculateSecondMin(int node) {
    int firstMin = INT_MAX, secondMin = INT_MAX;
    for (int j = 0; j < nodeCount; j++) {
        if (node == j) continue;
        if (distanceMatrix[node][j] <= firstMin) {
            secondMin = firstMin;
            firstMin = distanceMatrix[node][j];
        }
        else if (distanceMatrix[node][j] <= secondMin && distanceMatrix[node][j] != firstMin) {
            secondMin = distanceMatrix[node][j];
        }
    }
    return secondMin;
}

void findOptimalTour(int currentBound, int currentWeight, int level, int currentPath[]) {
    if (level == nodeCount) {
        if (distanceMatrix[currentPath[level-1]][currentPath[0]] != 0) {
            int currentCost = currentWeight + distanceMatrix[currentPath[level-1]][currentPath[0]];
            if (currentCost < minTourCost) {
                saveOptimalPath(currentPath);
                minTourCost = currentCost;
            }
        }
        return;
    }

    for (int i = 0; i < nodeCount; i++) {
        if (distanceMatrix[currentPath[level-1]][i] != 0 && !visitedNodes[i]) {
            int tempBound = currentBound;
            currentWeight += distanceMatrix[currentPath[level-1]][i];

            if (level == 1)
                currentBound -= (calculateFirstMin(currentPath[level-1]) + calculateFirstMin(i)) / 2;
            else
                currentBound -= (calculateSecondMin(currentPath[level-1]) + calculateFirstMin(i)) / 2;

            if (currentBound + currentWeight < minTourCost) {
                currentPath[level] = i;
                visitedNodes[i] = true;
                findOptimalTour(currentBound, currentWeight, level + 1, currentPath);
            }

            currentWeight -= distanceMatrix[currentPath[level-1]][i];
            currentBound = tempBound;
            visitedNodes[i] = false;
        }
    }
}

void solveTSP() {
    int currentPath[nodeCount+1];
    int currentBound = 0;
    fill(currentPath, currentPath + nodeCount + 1, -1);
    fill(visitedNodes, visitedNodes + nodeCount, false);

    for (int i = 0; i < nodeCount; i++)
        currentBound += (calculateFirstMin(i) + calculateSecondMin(i));

    currentBound = (currentBound & 1) ? currentBound / 2 + 1 : currentBound / 2;

    visitedNodes[0] = true;
    currentPath[0] = 0;
    findOptimalTour(currentBound, 0, 1, currentPath);
}

int main() {
    string filename = "l3-1.txt";
    
    if (!readDistanceMatrix(filename)) {
        cout << "Press Enter to exit...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return 1;
    }

    cout << "Distance Matrix (" << nodeCount << "x" << nodeCount << "):\n";
    for (int i = 0; i < nodeCount; ++i) {
        for (int j = 0; j < nodeCount; ++j) {
            cout << distanceMatrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;

    solveTSP();

    cout << "Minimum tour cost: " << minTourCost << endl;
    cout << "Optimal path: ";
    for (int i = 0; i <= nodeCount; i++)
        cout << optimalPath[i] + 1 << " ";

    cout << "\n\nPress Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return 0;
}