// COMSC-210 | Lab 34 | Ahmad Dharhan
// Airline route network: nodes are airports, edges are direct flights,
// and weights are approximate flight times in hours.

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <climits>
using namespace std;

const int SIZE = 11;

// Airport codes and cities; the index in each array is the vertex number
const string CODES[SIZE] = {
    "SFO", "LAX", "SEA", "DEN", "PHX", "DFW", "ORD", "ATL", "JFK", "MIA", "BOS"
};
const string CITIES[SIZE] = {
    "San Francisco", "Los Angeles", "Seattle", "Denver", "Phoenix", "Dallas",
    "Chicago", "Atlanta", "New York", "Miami", "Boston"
};

// Builds a display label such as "SFO (San Francisco)"
string label(int v) {
    return CODES[v] + " (" + CITIES[v] + ")";
}

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;  // Creates alias 'Pair' for the pair<int,int> data type

class Graph {
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;

    // Graph Constructor
    Graph(vector<Edge> const &edges) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);

        // add edges to the undirected graph (a flight works in both directions)
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));
            // for an undirected graph, add an edge from dest to src also
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    // Print every airport and its direct flights with flight times
    void printGraph() {
        cout << "Airline Route Network:" << endl;
        cout << "================================" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << label(i) << " connects to:" << endl;
            for (Pair v : adjList[i])
                cout << "  -> " << label(v.first)
                     << " (Flight time: " << v.second << " hr)" << endl;
        }
    }

    // Iterative depth-first search using a stack:
    // follows one chain of connections as far as possible before backtracking
    void DFS(int start) {
        vector<bool> visited(SIZE, false);
        vector<bool> discovered(SIZE, false);   // used only to avoid repeating messages
        stack<int> s;

        s.push(start);
        discovered[start] = true;

        cout << endl << "Route Trace (DFS) from " << label(start) << ":" << endl;
        cout << "Purpose: Exploring one chain of connections as deep as possible" << endl;
        cout << "=======================================" << endl;

        while (!s.empty()) {
            int current = s.top();
            s.pop();

            if (visited[current])
                continue;               // skip airports already visited

            visited[current] = true;    // mark when popped, not when pushed
            cout << "Inspecting " << label(current) << endl;

            for (Pair neighbor : adjList[current]) {
                int next = neighbor.first;
                if (!visited[next]) {
                    if (!discovered[next]) {
                        discovered[next] = true;
                        cout << "  -> Possible connection to " << label(next)
                             << " - Flight time: " << neighbor.second << " hr" << endl;
                    }
                    s.push(next);
                }
            }
        }
    }

    // Breadth-first search using a queue:
    // visits airports in order of fewest connections from the start
    void BFS(int start) {
        vector<bool> visited(SIZE, false);
        queue<int> q;

        visited[start] = true;          // mark when enqueued
        q.push(start);

        cout << endl << "Layer-by-Layer Route Inspection (BFS) from " << label(start) << ":" << endl;
        cout << "Purpose: Finding airports by fewest connections from the origin" << endl;
        cout << "=================================================" << endl;

        while (!q.empty()) {
            int current = q.front();
            q.pop();
            cout << "Checking " << label(current) << endl;

            for (Pair neighbor : adjList[current]) {
                int next = neighbor.first;
                if (!visited[next]) {
                    visited[next] = true;
                    q.push(next);
                    cout << "  -> Next stop: " << label(next)
                         << " - Flight time: " << neighbor.second << " hr" << endl;
                }
            }
        }
    }

    // Dijkstra's algorithm: shortest flight time from start to every airport
    void shortestPath(int start) {
        vector<int> dist(SIZE, INT_MAX);    // best known distance to each airport
        // min-heap of (distance, vertex) pairs so the closest airport comes out first
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        dist[start] = 0;
        pq.push(make_pair(0, start));

        while (!pq.empty()) {
            int d = pq.top().first;
            int current = pq.top().second;
            pq.pop();

            if (d > dist[current])
                continue;                   // outdated queue entry, skip it

            for (Pair neighbor : adjList[current]) {
                int next = neighbor.first;
                int weight = neighbor.second;

                // found a shorter way to reach 'next' through 'current'
                if (dist[current] + weight < dist[next]) {
                    dist[next] = dist[current] + weight;
                    pq.push(make_pair(dist[next], next));
                }
            }
        }

        cout << endl << "Shortest path from " << label(start) << ":" << endl;
        for (int i = 0; i < SIZE; i++)
            cout << CODES[start] << " -> " << CODES[i] << " : " << dist[i] << " hr" << endl;
    }

    // Prim's algorithm: minimum spanning tree, the cheapest set of flights
    // that still connects every airport (starting from the given airport)
    void minimumSpanningTree(int start) {
        vector<bool> inTree(SIZE, false);
        // min-heap of (weight, (airport, airport it is reached from)), cheapest flight first
        priority_queue<pair<int, Pair>, vector<pair<int, Pair>>, greater<pair<int, Pair>>> pq;

        cout << endl << "Minimum Spanning Tree edges:" << endl;

        pq.push(make_pair(0, make_pair(start, -1)));    // -1 means the start has no parent

        while (!pq.empty()) {
            int weight = pq.top().first;
            int current = pq.top().second.first;
            int parent = pq.top().second.second;
            pq.pop();

            if (inTree[current])
                continue;                   // airport already connected, skip

            inTree[current] = true;

            if (parent != -1) {             // the start airport has no incoming flight
                cout << "Edge from " << CODES[current] << " to " << CODES[parent]
                     << " with flight time: " << weight << " hr" << endl;
            }

            // offer every flight from this airport to airports not yet in the tree
            for (Pair neighbor : adjList[current]) {
                if (!inTree[neighbor.first])
                    pq.push(make_pair(neighbor.second, make_pair(neighbor.first, current)));
            }
        }
    }
};

int main() {
    // Direct flights: (x, y, w) —> flight between airports x and y taking about w hours
    vector<Edge> edges = {
        {0,1,1},   // SFO - LAX
        {0,2,2},   // SFO - SEA
        {0,3,2},   // SFO - DEN
        {1,4,1},   // LAX - PHX
        {1,3,2},   // LAX - DEN
        {2,6,4},   // SEA - ORD
        {3,6,2},   // DEN - ORD
        {4,5,2},   // PHX - DFW
        {5,7,2},   // DFW - ATL
        {5,6,2},   // DFW - ORD
        {6,8,2},   // ORD - JFK
        {7,9,2},   // ATL - MIA
        {7,8,2},   // ATL - JFK
        {8,10,1},  // JFK - BOS
        {8,9,3}    // JFK - MIA
    };

    // Creates graph
    Graph graph(edges);

    // Menu-driven driver; every option starts from San Francisco (vertex 0)
    int choice;
    do {
        cout << endl << "Airline Route Network Menu:" << endl;
        cout << "[1] Display airline route network" << endl;
        cout << "[2] Check reachable airports by connections (BFS)" << endl;
        cout << "[3] Trace route chains (DFS)" << endl;
        cout << "[4] Calculate shortest paths" << endl;
        cout << "[5] Find Minimum Spanning Tree" << endl;
        cout << "[0] Exit" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            if (cin.eof())
                break;                      // input ended, stop the menu
            cin.clear();                    // bad input (not a number): reset and ignore it
            cin.ignore(10000, '\n');
            choice = -1;
        }

        switch (choice) {
            case 1:
                graph.printGraph();
                break;
            case 2:
                graph.BFS(0);
                break;
            case 3:
                graph.DFS(0);
                break;
            case 4:
                graph.shortestPath(0);
                break;
            case 5:
                graph.minimumSpanningTree(0);
                break;
            case 0:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a number from 0 to 5." << endl;
        }
    } while (choice != 0);

    return 0;
}