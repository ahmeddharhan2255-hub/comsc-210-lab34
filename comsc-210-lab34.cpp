// COMSC-210 | Lab 34 | Ahmad Dharhan

#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

const int SIZE = 11;

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

        // add edges to the directed graph
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

    // Print the graph's adjacency list
    void printGraph() {
        cout << "Graph's adjacency list:" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << " --> ";
            for (Pair v : adjList[i])
                cout << "(" << v.first << ", " << v.second << ") ";
            cout << endl;
        }
    }

    void DFS(int start){
        vector<bool> visited(SIZE, false);
        stack<int> s;
        s.push(start);


        cout << "DFS starting from vertex " << start << ": ";

        while(!s.empty()){
            int current = s.top();
            s.pop();

            if (visited[current])
                continue;

            visited[current] = true;
            cout << current << " ";

            for (Pair neighbor: adjList[current]){
                if(!visited[neighbor.first])
                    s.push(neighbor.first);
            }
        }

        cout << endl;
    }

    void BFS(int start){
        vector<bool> visited(SIZE, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "BFS starting from vertex " << start << ": ";
        while(!q.empty()){
            int current = q.front();
            q.pop();
            cout << current << " ";

            for (Pair neighbor : adjList[current]){
                int next = neighbor.first;
                if(!visited[next]){
                    visited[next] = true;
                    q.push(next);
                }
            }
        }

        cout << endl;

    }
};

int main() {
    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
    // (x, y, w) —> edge from x to y having weight w
        {0,1,15},{0,2,9},{1,2,4},{1,3,3},{1,4,7},{3,4,11},{2,5,13},
        {3,6,5},{4,7,10},{5,8,6},{6,9,14},{7,10,2},{8,9,8},{9,10,12}
    };

    // Creates graph
    Graph graph(edges);

    // Prints adjacency list representation of graph
    graph.printGraph();
    cout << endl;

    graph.DFS(0);
    graph.BFS(0);

    return 0;
}



