#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main() {
    // Open the input file
    ifstream infile("input25.txt");
    if (!infile) {
        cerr << "Error: Unable to open input25.txt" << endl;
        return 1;
    }

    // Read all schematics separated by empty lines
    vector<vector<string>> schematics; // Each schematic is a vector of strings (rows)
    vector<string> current_schematic;
    string line;
    while (getline(infile, line)) {
        if (line.empty()) {
            if (!current_schematic.empty()) {
                schematics.push_back(current_schematic);
                current_schematic.clear();
            }
        }
        else {
            current_schematic.push_back(line);
        }
    }
    // Add the last schematic if the file doesn't end with an empty line
    if (!current_schematic.empty()) {
        schematics.push_back(current_schematic);
    }
    infile.close();

    // Check if any schematics were found
    if (schematics.empty()) {
        cerr << "Error: No schematics found in input25.txt" << endl;
        return 1;
    }

    // Determine the number of rows and columns from the first schematic
    int num_rows = schematics[0].size();
    int num_cols = schematics[0][0].size();

    // Containers for locks and keys heights
    vector<vector<int>> locks; // Each lock is a vector of heights per column
    vector<vector<int>> keys;  // Each key is a vector of heights per column

    // Process each schematic
    for (const auto& schem : schematics) {
        // Determine if it's a lock or a key based on the first character of the first row
        if (schem[0][0] == '#') {
            // It's a lock
            vector<int> lock_heights;
            for (int c = 0; c < num_cols; ++c) {
                int count = 0;
                for (int r = 0; r < num_rows; ++r) {
                    if (schem[r][c] == '#') {
                        count++;
                    }
                }
                // Height is the number of '#'s minus one
                lock_heights.push_back(count - 1);
            }
            locks.push_back(lock_heights);
        }
        else if (schem[0][0] == '.') {
            // It's a key
            vector<int> key_heights;
            for (int c = 0; c < num_cols; ++c) {
                int count = 0;
                for (int r = num_rows - 1; r >= 0; --r) {
                    if (schem[r][c] == '#') {
                        count++;
                    }
                }
                // Height is the number of '#'s minus one
                key_heights.push_back(count - 1);
            }
            keys.push_back(key_heights);
        }
        else {
            // Invalid schematic format
            cerr << "Warning: Schematic does not start with '#' or '.' and will be skipped." << endl;
        }
    }

    // Determine the available space
    // Assuming all schematics have the same number of rows
    int available_space = num_rows - 1;

    // Initialize the count of valid lock-key pairs
    int valid_pairs = 0;

    // Iterate through all lock-key pairs
    for (const auto& lock : locks) {
        for (const auto& key : keys) {
            bool fits = true; // Assume it fits until proven otherwise
            for (int c = 0; c < num_cols; ++c) {
                if ((lock[c] + key[c]) >= available_space) {
                    fits = false; // Overlap detected in this column
                    break; // No need to check further columns
                }
            }
            if (fits) {
                valid_pairs++; // Increment if the pair fits without overlaps
            }
        }
    }

    // Output the result
    cout << valid_pairs << endl;

    return 0;
}
