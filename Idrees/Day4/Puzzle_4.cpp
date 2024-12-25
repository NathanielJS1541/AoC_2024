#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Function to check if the word exists starting from (row, col) in a specific direction
bool checkWord(const vector<string>& grid, int row, int col, const string& word, int deltaRow, int deltaCol) {
   int numRows = grid.size();
   int numCols = grid[0].size();
   int wordLength = word.length();

   // Check boundaries
   int endRow = row + (wordLength - 1) * deltaRow;
   int endCol = col + (wordLength - 1) * deltaCol;

   if (endRow < 0 || endRow >= numRows || endCol < 0 || endCol >= numCols)
       return false;

   for (int i = 0; i < wordLength; ++i) {
       if (grid[row + i * deltaRow][col + i * deltaCol] != word[i])
           return false;
   }
   return true;
}

int countXMAS(const vector<string>& grid) {
   int count = 0;
   string target = "XMAS";
   int wordLength = target.length();

   // Directions: N, NE, E, SE, S, SW, W, NW
   vector<pair<int, int>> directions = {
       {-1,  0}, // North
       {-1,  1}, // Northeast
       { 0,  1}, // East
       { 1,  1}, // Southeast
       { 1,  0}, // South
       { 1, -1}, // Southwest
       { 0, -1}, // West
       {-1, -1}  // Northwest
   };

   int numRows = grid.size();
   if (numRows == 0) return 0;
   int numCols = grid[0].size();

   for (int row = 0; row < numRows; ++row) {
       for (int col = 0; col < numCols; ++col) {
           // Check each direction from the current cell
           for (const auto& dir : directions) {
               if (checkWord(grid, row, col, target, dir.first, dir.second)) {
                   count++;
               }
           }
       }
   }

   return count;
}

// Function to check if the given position is a valid X-MAS center
bool isXMAS(const vector<string>& grid, int row, int col) {
   // Check if the center is 'A'
   if (grid[row][col] != 'A') return false;

   // Check boundaries to prevent out-of-range access
   if (row - 1 < 0 || row + 1 >= grid.size() ||
       col - 1 < 0 || col + 1 >= grid[0].size()) {
       return false;
   }

   // Extract characters from the diagonals
   char topLeft = grid[row - 1][col - 1];
   char topRight = grid[row - 1][col + 1];
   char bottomLeft = grid[row + 1][col - 1];
   char bottomRight = grid[row + 1][col + 1];

   // Function to check if two characters form "MAS" or "SAM" sequences with 'A' in the center
   auto isValidPair = [&](char a, char b) -> bool {
       return (a == 'M' && b == 'S') || (a == 'S' && b == 'M');
       };

   // Check both diagonals
   bool firstDiagonal = isValidPair(topLeft, bottomRight);
   bool secondDiagonal = isValidPair(topRight, bottomLeft);

   return firstDiagonal && secondDiagonal;
}

// Function to count all X-MAS patterns in the grid
int countX_MAS(const vector<string>& grid) {
   int count = 0;
   int numRows = grid.size();
   if (numRows == 0) return 0;
   int numCols = grid[0].size();

   // Iterate through each cell, skipping the borders since an X-MAS requires a 3x3 area
   for (int i = 1; i < numRows - 1; ++i) {
       for (int j = 1; j < numCols - 1; ++j) {
           if (isXMAS(grid, i, j)) {
               count++;
           }
       }
   }

   return count;
}

int main() {
   string filename = "input04.txt";
   ifstream inputFile(filename);
   if (!inputFile) {
       cout << "Error: Unable to open file '" << filename << "'. Please ensure the file exists.\n";
       return 1;
   }

   vector<string> grid;
   string line;
   while (getline(inputFile, line)) {
       // Optional: Ensure all lines are the same length for a proper grid
       grid.push_back(line);
   }
   inputFile.close();

   // Optional: Validate grid consistency
   if (grid.empty()) {
       cout << "The input grid is empty.\n";
       return 1;
   }
   size_t expectedLength = grid[0].size();
   for (const auto& row : grid) {
       if (row.size() != expectedLength) {
           cout << "Error: Inconsistent row lengths in the grid.\n";
           return 1;
       }
   }

   int totalOccurrences = countXMAS(grid);
   cout << "Total occurrences of 'XMAS': " << totalOccurrences << endl;


   int totalXMAS = countX_MAS(grid);
   cout << "Total X-MAS patterns found: " << totalXMAS << endl;

   return 0;
}


