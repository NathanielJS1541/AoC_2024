


#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <cstring> // For memset

using namespace std;

// Structure to represent a position on the grid
struct Position {
   int row;
   int col;
};

// Function to perform BFS from a given trailhead and count reachable height 9 cells
int bfs_score(const vector<vector<int>>& grid, const Position& start) {
   int rows = grid.size();
   int cols = grid[0].size();

   // Directions: up, down, left, right
   const vector<pair<int, int>> directions = {
       {-1, 0}, {1, 0}, {0, -1}, {0, 1}
   };

   // Queue for BFS
   queue<Position> q;
   q.push(start);

   // Visited matrix to keep track of visited positions
   vector<vector<bool>> visited(rows, vector<bool>(cols, false));
   visited[start.row][start.col] = true;

   // Set to store reachable height 9 positions
   // Using a simple counter instead of a set for efficiency
   int reachableHeight9Count = 0;

   while (!q.empty()) {
       Position current = q.front();
       q.pop();

       // If current position is height 9, increment the counter
       if (grid[current.row][current.col] == 9) {
           reachableHeight9Count++;
           // Do not continue from height 9 as we cannot go higher
           continue;
       }

       // Explore all four directions
       for (const auto& dir : directions) {
           int newRow = current.row + dir.first;
           int newCol = current.col + dir.second;

           // Check boundaries
           if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols)
               continue;

           // Check if already visited
           if (visited[newRow][newCol])
               continue;

           // Check if the next cell's height is exactly 1 greater
           if (grid[newRow][newCol] == grid[current.row][current.col] + 1) {
               visited[newRow][newCol] = true;
               q.push(Position{ newRow, newCol });
           }
       }
   }

   return reachableHeight9Count;
}

// Function to perform DFS with memoization to count distinct hiking trails
long long dfs_rating(const vector<vector<int>>& grid, const Position& current, vector<vector<long long>>& memo) {
   int rows = grid.size();
   int cols = grid[0].size();

   // If already computed, return the stored value
   if (memo[current.row][current.col] != -1)
       return memo[current.row][current.col];

   // If current position is height 9, there's exactly one path (ending here)
   if (grid[current.row][current.col] == 9)
       return memo[current.row][current.col] = 1;

   // Directions: up, down, left, right
   const vector<pair<int, int>> directions = {
       {-1, 0}, {1, 0}, {0, -1}, {0, 1}
   };

   long long paths = 0;

   // Explore all four directions
   for (const auto& dir : directions) {
       int newRow = current.row + dir.first;
       int newCol = current.col + dir.second;

       // Check boundaries
       if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols)
           continue;

       // Check if the next cell's height is exactly 1 greater
       if (grid[newRow][newCol] == grid[current.row][current.col] + 1) {
           Position next = { newRow, newCol };
           paths += dfs_rating(grid, next, memo);
       }
   }

   return memo[current.row][current.col] = paths;
}

int main() {
   ifstream inputFile("input10.txt");
   if (!inputFile.is_open()) {
       cerr << "Error: Unable to open input10.txt" << endl;
       return 1;
   }

   vector<vector<int>> grid;
   string line;

   // Read the grid from the file
   while (getline(inputFile, line)) {
       vector<int> row;
       for (char ch : line) {
           if (ch >= '0' && ch <= '9') {
               row.push_back(ch - '0');
           }
           // If the character is not a digit, it is ignored.
           // Ensure that the input file contains only digits.
       }
       if (!row.empty()) {
           grid.push_back(row);
       }
   }

   inputFile.close();

   if (grid.empty()) {
       cerr << "Error: The input grid is empty." << endl;
       return 1;
   }

   int rows = grid.size();
   int cols = grid[0].size();

   // Identify all trailheads (cells with height 0)
   vector<Position> trailheads;

   for (int r = 0; r < rows; ++r) {
       for (int c = 0; c < cols; ++c) {
           if (grid[r][c] == 0) {
               trailheads.push_back(Position{ r, c });
           }
       }
   }

   // If no trailheads are found
   if (trailheads.empty()) {
       cout << "No trailheads (height 0) found on the map." << endl;
       cout << "Sum of trailhead scores: 0" << endl;
       cout << "Sum of trailhead ratings: 0" << endl;
       return 0;
   }

   // Part One: Calculate the sum of all trailhead scores
   long long totalScore = 0;

   for (const auto& trailhead : trailheads) {
       int score = bfs_score(grid, trailhead);
       totalScore += score;
   }

   // Part Two: Calculate the sum of all trailhead ratings
   // Initialize memoization table with -1
   vector<vector<long long>> memo(rows, vector<long long>(cols, -1));

   long long totalRating = 0;

   for (const auto& trailhead : trailheads) {
       long long rating = dfs_rating(grid, trailhead, memo);
       totalRating += rating;
   }

   // Output the results
   cout << "Sum of the scores of all trailheads: " << totalScore << endl;
   cout << "Sum of the ratings of all trailheads: " << totalRating << endl;

   return 0;
}
