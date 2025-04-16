#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
using namespace std;

// Check if graph is Eulerian
bool isEulerian(const vector<vector<int>>& graph) {
    int n = graph.size();
    for (int i = 0; i < n; ++i) {
        int degree = 0;
        for (int j = 0; j < n; ++j) {
            if (graph[i][j] != 0) degree++;
        }
        if (degree % 2 != 0) return false;
    }
    return true;
}

// Find Eulerian cycle using Hierholzer's algorithm
void findEulerCycle(vector<vector<int>>& graph, int start, vector<int>& cycle) {
    for (int v = 0; v < graph.size(); ++v) {
        if (graph[start][v] != 0) {
            graph[start][v] = 0;  // Remove edge
            graph[v][start] = 0;
            findEulerCycle(graph, v, cycle);
        }
    }
    cycle.push_back(start);
}

// Read matrix from file (ignoring non-numeric headers)
vector<vector<int>> readMatrix(const string& filename) {
    ifstream file(filename);
    vector<vector<int>> matrix;
    string line;
    
    getline(file, line);
    
    // Read 8x8 matrix
    matrix.resize(8, vector<int>(8));
    for (int i = 0; i < 8; ++i) {        
        for (int j = 0; j < 8; ++j) {
            file >> matrix[i][j];
        }
    }
    return matrix;
}

int main() {
    vector<vector<int>> matrix = readMatrix("l2-1.txt");
    
    cout << "Adjacency Matrix:\n";
    for (const auto& row : matrix) {
        for (int val : row) cout << val << "\t";
        cout << endl;
    }

    if (!isEulerian(matrix)) {
        cout << "\nNo Eulerian cycle exists (odd degree vertices found)\n";
    } else {
        vector<int> cycle;
        findEulerCycle(matrix, 0, cycle);
        
        cout << "\nEulerian Cycle (1-based): ";
        for (int v : cycle) cout << v + 1 << " ";
        cout << endl;
    }

    cout << "\nPress Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    
    return 0;
}