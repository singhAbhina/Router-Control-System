#include <iostream>
#include<bits/stdc++.h>
#include <vector>
#include <queue>
#include <climits>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <functional>

using namespace std;

typedef pair<int, int> pii;

// Function declarations
void displayNetwork(const vector<vector<pii>>& adj, int n, const vector<string>& ipAddresses, const vector<string>& macAddresses, ofstream& outFile);

vector<int> dijkstra(int src, const vector<vector<pii>>& adj, int n, vector<int>& prev);

void primMST(const vector<vector<pii>>& adj, int n, ofstream& outFile);

vector<pii> findCriticalConnections(const vector<vector<pii>>& adj, int n);

void displayCriticalConnections(const vector<pii>& bridges, ofstream& outFile);

void addNode(vector<vector<pii>>& adj, int& n, const string& ip, const string& mac, vector<string>& ipAddresses, vector<string>& macAddresses);

void removeNode(vector<vector<pii>>& adj, int& n, int node, ofstream& outFile, const vector<string>& ipAddresses, const vector<string>& macAddresses);

void displayRoutingTable(const vector<vector<pii>>& adj, int n, const vector<string>& ipAddresses, const vector<string>& macAddresses, ofstream& outFile);

void printShortestPaths(int src, const vector<int>& dist, const vector<int>& prev, ofstream& outFile);

void printPath(int src, int dest, const vector<int>& prev, ofstream& outfile);

void controlPanel(vector<vector<pii>>& adj, int& n, vector<string>& ipAddresses, vector<string>& macAddresses);



string generateIPAddress(int routerID) {
    stringstream ss;
    ss << "192.168.1." << routerID;
    return ss.str();
}

string generateMACAddress(int routerID) {
    stringstream ss;
    ss << "00:1A:2B:3C:4D:" << hex << (routerID < 16 ? "0" : "") << routerID;
    return ss.str();
}


void removeConnection(vector<vector<pii>>& adj, int u, int v) {
    adj[u].erase(remove_if(adj[u].begin(), adj[u].end(), [v](const pii& edge) { return edge.second == v; }), adj[u].end());
    adj[v].erase(remove_if(adj[v].begin(), adj[v].end(), [u](const pii& edge) { return edge.second == u; }), adj[v].end());
}


vector<int> dijkstra(int src, const vector<vector<pii>>& adj, int n, vector<int>& prev) {
    vector<int> dist(n, INT_MAX);
    priority_queue<pii, vector<pii>, greater<pii>> pq;

    dist[src] = 0;
    pq.push({0, src});
    prev[src] = -1;

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (auto& neighbor : adj[u]) {
            int v = neighbor.second;
            int weight = neighbor.first;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
                prev[v] = u;
            }
        }
    }

    return dist;
}

// Prim's algorithm to find Minimum Spanning Tree (MST)
void primMST(const vector<vector<pii>>& adj, int n, ofstream& outFile) {
    vector<int> key(n, INT_MAX);
    vector<bool> inMST(n, false);
    priority_queue<pii, vector<pii>, greater<pii>> pq;

    key[0] = 0;
    pq.push({0, 0});
    vector<int> parent(n, -1);

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        inMST[u] = true;

        for (auto& neighbor : adj[u]) {
            int v = neighbor.second;
            int weight = neighbor.first;

            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                pq.push({key[v], v});
                parent[v] = u;
            }
        }
    }

    outFile << "\n=== Minimum Spanning Tree (MST) ===\n";
    outFile << "------------------------------------\n";
    for (int i = 1; i < n; i++) {
        outFile << "Router " << parent[i] << " -- Router " << i << " (Weight: " << key[i] << ")\n";
    }
}

// Function to display critical connections (bridges)
vector<pii> findCriticalConnections(const vector<vector<pii>>& adj, int n) {
    vector<int> disc(n, -1);
    vector<int> low(n, -1);
    vector<bool> visited(n, false);
    vector<pii> bridges;
    int time = 0;

    function<void(int, int)> dfs = [&](int u, int parent) {
        visited[u] = true;
        disc[u] = low[u] = ++time;
        for (const auto& neighbor : adj[u]) {
            int v = neighbor.second;
            if (v == parent) continue;
            if (!visited[v]) {
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] > disc[u]) bridges.push_back({u, v});
            } else {
                low[u] = min(low[u], disc[v]);
            }
        }
    };

    for (int i = 0; i < n; i++) {
        if (!visited[i]) dfs(i, -1);
    }

    return bridges;
}

void displayCriticalConnections(const vector<pii>& bridges, ofstream& outFile) {
    outFile << "\n=== Critical Connections (Bridges) ===\n";
    outFile << "--------------------------------------\n";
    for (const auto& bridge : bridges) {
        outFile << "Router " << bridge.first << " -- Router " << bridge.second << " is a critical connection.\n";
    }
    outFile << "Total critical connections: " << bridges.size() << "\n";
}

// Function to display network topology
void displayNetwork(const vector<vector<pii>>& adj, int n, const vector<string>& ipAddresses, const vector<string>& macAddresses, ofstream& outFile) {
    outFile << "\n=== Network Topology ===\n";
    outFile << "------------------------\n";
    for (int i = 0; i < n; i++) {
        outFile << "Router " << i << " [IP: " << ipAddresses[i] << ", MAC: " << macAddresses[i] << "]\n";
        for (const auto& neighbor : adj[i]) {
            outFile << "  -> Connected to Router " << neighbor.second << " with weight " << neighbor.first << "\n";
        }
    }
}


void displayRoutingTable(const vector<vector<pii>>& adj, int n, const vector<string>& ipAddresses, const vector<string>& macAddresses, ofstream& outFile) {
    outFile << "\n=== Routing Table ===\n";
    outFile << "---------------------\n";
    for (int i = 0; i < n; i++) {
        outFile << "Router " << i << " [IP: " << ipAddresses[i] << ", MAC: " << macAddresses[i] << "]\n";
        outFile << "Next hops:\n";
        for (const auto& neighbor : adj[i]) {
            outFile << "  -> Router " << neighbor.second << "\n";
        }
    }
}


void printShortestPaths(int src, const vector<int>& dist, const vector<int>& prev, ofstream& outFile) {
    outFile << "\n=== Shortest Paths from Router " << src << " ===\n";
    outFile << "--------------------------------\n";
    for (int i = 0; i < dist.size(); i++) {
        outFile << "Shortest path to Router " << i << ": ";
        printPath(src, i, prev, outFile);
        outFile << " with a distance of " << dist[i] << "\n";
    }
}

// Function to print the shortest path between two nodes
void printPath(int src, int dest, const vector<int>& prev, ofstream& outfile) {
    if (dest == -1) {
        outfile << "No path";
        return;
    }
    if (dest == src) {
        outfile << src;
        return;
    }
    printPath(src, prev[dest], prev, outfile);
    outfile << " -> " << dest;
}


void addNode(vector<vector<pii>>& adj, int& n, const string& ip, const string& mac, vector<string>& ipAddresses, vector<string>& macAddresses) {
    adj.push_back({});
    ipAddresses.push_back(ip);
    macAddresses.push_back(mac);

    cout << "Enter the number of connections for the new router: ";
    int numConnections;
    cin >> numConnections;

    for (int i = 0; i < numConnections; i++) {
        int targetRouter, weight;
        cout << "Enter the target router and the weight of the connection: ";
        cin >> targetRouter >> weight;
        adj[n].push_back({weight, targetRouter});
        adj[targetRouter].push_back({weight, n}); 
    }

    n++;
}


void removeNode(vector<vector<pii>>& adj, int& n, int node, ofstream& outFile, vector<string>& ipAddresses, vector<string>& macAddresses) {
    if (node >= n) {
        outFile << "Router " << node << " does not exist.\n";
        return;
    }

   
    for (int i = 0; i < n; i++) {
        removeConnection(adj, i, node);
    }

    
    ipAddresses.erase(ipAddresses.begin() + node);
    macAddresses.erase(macAddresses.begin() + node);
    adj.erase(adj.begin() + node);
    n--;

    
    for (int i = 0; i < n; i++) {
        for (auto& neighbor : adj[i]) {
            if (neighbor.second > node) {
                neighbor.second--; 
            }
        }
    }

    outFile << "Router " << node << " removed.\n";
}


// Control panel 
void controlPanel(vector<vector<pii>>& adj, int& n, vector<string>& ipAddresses, vector<string>& macAddresses) {
    ofstream outFile("network_output.txt");

    while (true) {
        cout << "\nRouter Network Control Panel\n";
        cout << "1. Display network topology\n";
        cout << "2. Compute shortest paths (Dijkstra)\n";
        cout << "3. Compute Minimum Spanning Tree (Prim)\n";
        cout << "4. Detect critical connections\n";
        cout << "5. Add a router\n";
        cout << "6. Remove a router\n";
        cout << "7. Display routing table\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            displayNetwork(adj, n, ipAddresses, macAddresses, outFile);
        } else if (choice == 2) {
            cout << "Enter source router: ";
            int src;
            cin >> src;
            vector<int> prev(n, -1);
            vector<int> dist = dijkstra(src, adj, n, prev);
            printShortestPaths(src, dist, prev, outFile);
        } else if (choice == 3) {
            primMST(adj, n, outFile);
        } else if (choice == 4) {
            vector<pii> bridges = findCriticalConnections(adj, n);
            displayCriticalConnections(bridges, outFile);
        } else if (choice == 5) {
            cout << "Adding a new router...\n";
            string newIP = generateIPAddress(n);
            string newMAC = generateMACAddress(n);
            addNode(adj, n, newIP, newMAC, ipAddresses, macAddresses);
            outFile << "Router added with IP: " << newIP << ", MAC: " << newMAC << "\n";
        } else if (choice == 6) {
            cout << "Enter router ID to remove: ";
            int node;
            cin >> node;
            removeNode(adj, n, node, outFile, ipAddresses, macAddresses);
        } else if (choice == 7) {
            displayRoutingTable(adj, n, ipAddresses, macAddresses, outFile);
        } else if (choice == 8) {
            outFile.close();
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }
}


int main() {
    int n = 7; 
    vector<vector<pii>> adj(n);
    vector<string> ipAddresses, macAddresses;

    
    for (int i = 0; i < n; i++) {
        string ip = generateIPAddress(i);
        string mac = generateMACAddress(i);
        ipAddresses.push_back(ip);
        macAddresses.push_back(mac);
    }

    
    
    adj[0].push_back({10, 1}); 
    adj[1].push_back({10, 0}); 
    adj[1].push_back({15, 2}); 
    adj[2].push_back({15, 1}); 
    adj[2].push_back({9, 4});
    adj[2].push_back({5, 3});  
    adj[3].push_back({5, 2});  
    adj[3].push_back({20, 4}); 
    adj[4].push_back({20, 3}); 
    adj[4].push_back({25, 5}); 
    adj[5].push_back({25, 4}); 
    adj[5].push_back({30, 6}); 
   adj[4].push_back({23, 0});
    adj[6].push_back({30, 5}); 

   
    controlPanel(adj, n, ipAddresses, macAddresses);

    return 0;
}


