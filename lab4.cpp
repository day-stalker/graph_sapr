#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
#include <limits>
using namespace std;

bool bfs(vector<vector<int>>& residualGraph, vector<int>& parent, int s, int t) {
    int V = residualGraph.size();
    vector<bool> visited(V, false);
    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < V; v++) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                if (v == t) {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return false;
}

int fordFulkerson(vector<vector<int>>& graph, int s, int t) {
    int V = graph.size();
    vector<vector<int>> residualGraph = graph;
    vector<int> parent(V);
    int max_flow = 0;

    while (bfs(residualGraph, parent, s, t)) {
        int path_flow = INT_MAX;
        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            path_flow = min(path_flow, residualGraph[u][v]);
        }

        for (int v = t; v != s; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= path_flow;
            residualGraph[v][u] += path_flow;
        }

        max_flow += path_flow;
    }
    return max_flow;
}

vector<vector<int>> readGraphFromFile(const string& filename, int& V) {
    ifstream file(filename);
    vector<vector<int>> graph;
    
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return graph;
    }

    file >> V;
    graph.resize(V, vector<int>(V));
    
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            file >> graph[i][j];
        }
    }
    return graph;
}

void printMatrix(const vector<vector<int>>& matrix) {
    cout << "Adjacency Matrix:\n";
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << "\t";
        }
        cout << endl;
    }
}

int main() {
    int V;
    string filename = "l4-1.txt";
    
    vector<vector<int>> graph = readGraphFromFile(filename, V);
    if (graph.empty()) {
        cout << "Press Enter to exit...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        return 1;
    }

    printMatrix(graph);
    
    int source = 0;  // First vertex
    int sink = V-1;  // Last vertex
    
    cout << "\nCalculating maximum flow from " << source+1 << " to " << sink+1 << "...\n";
    int max_flow = fordFulkerson(graph, source, sink);
    
    cout << "\nMaximum flow: " << max_flow << endl;
    
    cout << "\nPress Enter to exit...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    
    return 0;
}