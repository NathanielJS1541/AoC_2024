// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <unordered_map>
// #include <unordered_set>
// #include <vector>
// #include <algorithm>

// // Function to check if at least one name starts with 't' or 'T'
// bool hasNameStartingWithT(const vector<string>& trio) {
//     for (const auto& name : trio) {
//         if (!name.empty() && (name[0] == 't' || name[0] == 'T')) {
//             return true;
//         }
//     }
//     return false;
// }

// using namespace std;

// int main() {
//     // Open the input file
//     ifstream infile("input23.txt");
//     if (!infile) {
//         cerr << "Error: Unable to open input23.txt" << endl;
//         return 1;
//     }

//     // Adjacency map: computer -> set of connected computers
//     unordered_map<string, unordered_set<string>> adjacency;

//     string line;
//     while (getline(infile, line)) {
//         // Ignore empty lines
//         if (line.empty()) continue;

//         // Split the line into two computer names
//         stringstream ss(line);
//         string comp1, comp2;
//         if (getline(ss, comp1, '-') && getline(ss, comp2)) {
//             // Add the connection both ways since the graph is undirected
//             adjacency[comp1].insert(comp2);
//             adjacency[comp2].insert(comp1);
//         }
//     }

//     infile.close();

//     // To store unique triangles
//     vector<vector<string>> triangles;

//     // Iterate through each computer and its neighbors to find triangles
//     for (const auto& [comp, neighbors] : adjacency) {
//         // For each pair of neighbors, check if they are connected
//         for (auto it1 = neighbors.begin(); it1 != neighbors.end(); ++it1) {
//             for (auto it2 = next(it1); it2 != neighbors.end(); ++it2) {
//                 const string& neighbor1 = *it1;
//                 const string& neighbor2 = *it2;
//                 // Check if neighbor1 and neighbor2 are connected
//                 if (adjacency[neighbor1].find(neighbor2) != adjacency[neighbor1].end()) {
//                     // Create a sorted trio to avoid duplicates
//                     vector<string> trio = { comp, neighbor1, neighbor2 };
//                     sort(trio.begin(), trio.end());
//                     triangles.emplace_back(trio);
//                 }
//             }
//         }
//     }

//     // Remove duplicate triangles
//     sort(triangles.begin(), triangles.end());
//     triangles.erase(unique(triangles.begin(), triangles.end()), triangles.end());

//     // Count triangles with at least one name starting with 't' or 'T'
//     int count = 0;
//     for (const auto& trio : triangles) {
//         if (hasNameStartingWithT(trio)) {
//             count++;
//             // Optional: Print the triangle
//             // cout << trio[0] << "," << trio[1] << "," << trio[2] << endl;
//         }
//     }

//     // Output the result
//     cout << "Number of triangles with at least one computer starting with 't': " << count << endl;

//     return 0;
// }






#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

// Function to check if at least one name starts with 't' or 'T'
bool hasNameStartingWithT(const vector<string>& trio) {
    for (const auto& name : trio) {
        if (!name.empty() && (name[0] == 't' || name[0] == 'T')) {
            return true;
        }
    }
    return false;
}

using namespace std;

// Function to read the network map and build the adjacency list
unordered_map<string, unordered_set<string>> buildAdjacencyList(const string& filename) {
    unordered_map<string, unordered_set<string>> adjacency;
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: Unable to open " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(infile, line)) {
        // Ignore empty lines
        if (line.empty()) continue;

        // Split the line into two computer names
        stringstream ss(line);
        string comp1, comp2;
        if (getline(ss, comp1, '-') && getline(ss, comp2)) {
            // Add the connection both ways since the graph is undirected
            adjacency[comp1].insert(comp2);
            adjacency[comp2].insert(comp1);
        }
    }

    infile.close();
    return adjacency;
}

// Function to find the maximum clique using the Bron–Kerbosch algorithm with pivot selection
void bronKerbosch(
    const unordered_map<string, unordered_set<string>>& adjacency,
    unordered_set<string> R,
    unordered_set<string> P,
    unordered_set<string> X,
    vector<string>& maxClique
) {
    if (P.empty() && X.empty()) {
        // Found a maximal clique
        if (R.size() > maxClique.size()) {
            maxClique.assign(R.begin(), R.end());
        }
        return;
    }

    // Choose a pivot from P ∪ X
    string pivot;
    unordered_set<string> unionPX;
    for (const auto& elem : P) unionPX.insert(elem);
    for (const auto& elem : X) unionPX.insert(elem);
    if (!unionPX.empty()) {
        pivot = *unionPX.begin(); // Simple pivot selection
    }

    // Candidates are P \ N(pivot)
    unordered_set<string> candidates;
    for (const auto& v : P) {
        if (adjacency.at(pivot).find(v) == adjacency.at(pivot).end()) {
            candidates.insert(v);
        }
    }

    for (const auto& v : candidates) {
        unordered_set<string> newR = R;
        newR.insert(v);

        unordered_set<string> newP;
        for (const auto& neighbor : adjacency.at(v)) {
            if (P.find(neighbor) != P.end()) {
                newP.insert(neighbor);
            }
        }

        unordered_set<string> newX;
        for (const auto& neighbor : adjacency.at(v)) {
            if (X.find(neighbor) != X.end()) {
                newX.insert(neighbor);
            }
        }

        bronKerbosch(adjacency, newR, newP, newX, maxClique);

        P.erase(v);
        X.insert(v);
    }
}

int main() {
    // Read the network map from input23.txt
    string filename = "input23.txt";
    auto adjacency = buildAdjacencyList(filename);

    // Extract all nodes
    unordered_set<string> nodes;
    for (const auto& [node, _] : adjacency) {
        nodes.insert(node);
    }

    // Initialize variables for the maximum clique search
    vector<string> maxClique;
    unordered_set<string> R; // Current clique
    unordered_set<string> P = nodes; // Potential candidates
    unordered_set<string> X; // Already processed

    // Start the recursive search
    bronKerbosch(adjacency, R, P, X, maxClique);

    // Sort the maxClique alphabetically
    sort(maxClique.begin(), maxClique.end());

    // Join the names with commas to form the password
    string password;
    for (size_t i = 0; i < maxClique.size(); ++i) {
        password += maxClique[i];
        if (i != maxClique.size() - 1) {
            password += ",";
        }
    }

    // Output the result
    cout << "Password to get into the LAN party: " << password << endl;

    return 0;
}

