#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

// Function to print adjacency matrix
void printMatrix(const vector<vector<int>>& matrix, const string& name) {
    cout << "Adjacency Matrix for " << name << ":\n";
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

bool areIsomorphic(vector<vector<int>>& g1, vector<vector<int>>& g2) {
    // Basic size check
    if (g1.size() != g2.size()) {
        cout << "Graphs have different number of vertices.\n";
        return false;
    }
    int n = g1.size();

    // 1. Check degree sequences (quick rejection)
    auto getDegreeSequence = [](vector<vector<int>>& g) {
        vector<int> degrees;
        for (auto& row : g) {
            degrees.push_back(count(row.begin(), row.end(), 1));
        }
        sort(degrees.begin(), degrees.end());
        return degrees;
    };

    vector<int> degSeq1 = getDegreeSequence(g1);
    vector<int> degSeq2 = getDegreeSequence(g2);

    cout << "Degree sequence for Graph 1: ";
    for (int d : degSeq1) cout << d << " ";
    cout << "\nDegree sequence for Graph 2: ";
    for (int d : degSeq2) cout << d << " ";
    cout << "\n\n";

    if (degSeq1 != degSeq2) {
        cout << "Degree sequences don't match.\n";
        return false;
    }

    // 2. Brute-force permutation check
    vector<int> permutation(n);
    for (int i = 0; i < n; i++) permutation[i] = i;

    cout << "Checking all possible permutations...\n";
    do {
        bool match = true;
        for (int i = 0; i < n && match; i++) {
            for (int j = 0; j < n && match; j++) {
                if (g1[i][j] != g2[permutation[i]][permutation[j]]) {
                    match = false;
                }
            }
        }
        if (match) {
            cout << "Found matching permutation!\n";
            return true;
        }
    } while (next_permutation(permutation.begin(), permutation.end()));

    return false;
}

int main() {
    // Define two test graphs
    vector<vector<int>> graph1 = {
        {0, 1, 0, 1},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {1, 0, 1, 0}
    };

    vector<vector<int>> graph2 = {
        {0, 0, 1, 1},
        {0, 0, 1, 1},
        {1, 1, 0, 0},
        {1, 1, 0, 0}
    };

    // Print both matrices
    printMatrix(graph1, "Graph 1");
    printMatrix(graph2, "Graph 2");

    // Check isomorphism
    cout << "Performing isomorphism check:\n";
    bool result = areIsomorphic(graph1, graph2);
    cout << "\nFinal result: ";
    if (result) {
        cout << "Graphs ARE isomorphic.\n";
    } else {
        cout << "Graphs are NOT isomorphic.\n";
    }

    // Pause before exit
    cout << "\nPress Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return 0;
}