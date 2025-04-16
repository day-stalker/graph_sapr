#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

typedef pair<int, int> pii; // {weight, vertex}

vector<vector<int>> readGraph() {
    ifstream file("l1-1.txt");
    vector<vector<int>> graph;
    string line;
    
    // Skip first line
    getline(file, line);
    
    // Read the matrix
    int num;
    while (file >> num) {
        vector<int> row;
        row.push_back(num);
        for (int i = 1; i < 8; i++) {
            file >> num;
            row.push_back(num);
        }
        graph.push_back(row);
    }
    return graph;
}

void primMST(const vector<vector<int>>& graph, bool isMaxST = false) {
    int n = graph.size();
    vector<bool> inMST(n, false);
    vector<int> key(n, isMaxST ? INT_MIN : INT_MAX);
    vector<int> parent(n, -1);
    priority_queue<pii, vector<pii>, greater<pii>> minHeap;
    priority_queue<pii> maxHeap;

    int start = 0;
    key[start] = 0;
    if (!isMaxST) minHeap.push({0, start});
    else maxHeap.push({0, start});

    while ((!isMaxST && !minHeap.empty()) || (isMaxST && !maxHeap.empty())) {
        int u = !isMaxST ? minHeap.top().second : maxHeap.top().second;
        if (!isMaxST) minHeap.pop(); else maxHeap.pop();
        inMST[u] = true;

        for (int v = 0; v < n; v++) {
            int weight = graph[u][v];
            if (weight && !inMST[v] && ((!isMaxST && weight < key[v]) || (isMaxST && weight > key[v]))) {
                key[v] = weight;
                parent[v] = u;
                if (!isMaxST) minHeap.push({key[v], v});
                else maxHeap.push({key[v], v});
            }
        }
    }

    cout << (isMaxST ? "MaxST" : "MST") << " edges:\n";
    int total = 0;
    for (int i = 1; i < n; i++) {
        if (parent[i] != -1) {
            cout << parent[i]+1 << "-" << i+1 << " (" << graph[parent[i]][i] << ")\n";
            total += graph[parent[i]][i];
        }
    }
    cout << "Total weight: " << total << "\n";
}

int main() {
    auto graph = readGraph();
    
    cout << "=== Minimal Spanning Tree ===\n";
    primMST(graph);
    
    cout << "\n=== Maximal Spanning Tree ===\n";
    primMST(graph, true);
    
    cout << "\nDone! Press Enter to exit...";
    cin.ignore();
    cin.get();
}