#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <fstream>
#include <queue>
#include <algorithm>

using namespace std;

// Function to perform topological sorting using Kahn's Algorithm
bool topologicalSort(const vector<int>& nodes, const unordered_map<int, unordered_set<int>>& adjList, vector<int>& sorted) {
   unordered_map<int, int> inDegree;
   for (auto node : nodes) {
       inDegree[node] = 0;  // Initialize in-degree for each node
   }

   // Calculate in-degrees based on filtered rules
   for (auto node : nodes) {
       if (adjList.count(node)) {
           for (auto dest : adjList.at(node)) {
               if (inDegree.count(dest)) {
                   inDegree[dest]++;
               }
           }
       }
   }

   queue<int> q;
   // Enqueue nodes with in-degree 0
   for (auto& pair : inDegree) {
       int node = pair.first;
       int degree = pair.second;
       if (degree == 0) {
           q.push(node);
       }
   }

   while (!q.empty()) {
       int node = q.front(); q.pop();
       sorted.push_back(node);
       if (adjList.count(node)) {
           for (auto neighbor : adjList.at(node)) {
               if (--inDegree[neighbor] == 0) {
                   q.push(neighbor);
               }
           }
       }
   }

   return sorted.size() == nodes.size();  // Check if there was a valid sort
}

int main() {
   string filename = "input05.txt";  // File containing rules and updates
   ifstream inputFile(filename);
   if (!inputFile.is_open()) {
       cerr << "Error opening file: " << filename << endl;
       return 1;
   }

   string line;
   unordered_map<int, unordered_set<int>> adjList;
   vector<vector<int>> updates;

   // Read the input
   while (getline(inputFile, line)) {
       if (line.find('|') != string::npos) {
           stringstream ss(line);
           int x, y;
           char delim;
           ss >> x >> delim >> y;
           adjList[x].insert(y);
       }
       else if (!line.empty()) {
           stringstream ss(line);
           vector<int> update;
           int page;
           while (ss >> page) {
               update.push_back(page);
               if (ss.peek() == ',')
                   ss.ignore();
           }
           updates.push_back(update);
       }
   }

   inputFile.close();

   int totalMiddlePagesCorrect = 0;
   int totalMiddlePagesFixed = 0;

   // Process each update
   for (auto& update : updates) {
       vector<int> sorted;
       if (topologicalSort(update, adjList, sorted) && sorted == update) {
           // Correct order
           int midIndex = update.size() / 2;
           totalMiddlePagesCorrect += update[midIndex];
       }
       else {
           // Fix the order using topological sort
           sorted.clear();
           topologicalSort(update, adjList, sorted);
           int midIndex = sorted.size() / 2;
           totalMiddlePagesFixed += sorted[midIndex];
       }
   }

   cout << "Total sum of middle pages from correctly ordered updates: " << totalMiddlePagesCorrect << endl;
   cout << "Total sum of middle pages from fixed updates: " << totalMiddlePagesFixed << endl;

   return 0;
}
