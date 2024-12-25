//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <queue>
//#include <string>
//#include <sstream>
//using namespace std;

//// Define the size of the grid
//const int GRID_SIZE = 71;
//
//// Directions for movement: up, down, left, right
//const vector<pair<int, int>> DIRECTIONS = {
//    {-1, 0}, // Up
//    {1, 0},  // Down
//    {0, -1}, // Left
//    {0, 1}   // Right
//};
//
//// Function to check if a position is within the grid bounds
//bool isValid(int x, int y) {
//    return (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE);
//}
//
//int main() {
//    // Initialize the grid with '.' representing safe cells
//   vector<vector<char>> grid(GRID_SIZE, vector<char>(GRID_SIZE, '.'));
//
//    // Open the input file
//    std::ifstream infile("input18.txt");
//    if (!infile.is_open()) {
//       cerr << "Error: Could not open input18.txt" << endl;
//        return 1;
//    }
//
//    string line;
//    int byte_count = 0;
//    const int MAX_BYTES = 1024;
//
//    // Read and parse each line in the input file
//    while (getline(infile, line) && byte_count < MAX_BYTES) {
//        // Remove any potential whitespace
//        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
//
//        // Skip empty lines
//        if (line.empty()) continue;
//
//        // Parse X and Y coordinates
//        stringstream ss(line);
//        string x_str, y_str;
//        if (getline(ss, x_str, ',') && getline(ss, y_str)) {
//            try {
//                int x = std::stoi(x_str);
//                int y = std::stoi(y_str);
//                if (isValid(x, y)) {
//                    grid[y][x] = '#'; // Mark corrupted cell
//                    byte_count++;
//                }
//                else {
//                   cerr << "Warning: Ignored out-of-bounds coordinate (" << x << "," << y << ")" << endl;
//                }
//            }
//            catch (const std::invalid_argument& e) {
//                cerr << "Warning: Invalid line format: " << line << endl;
//            }
//        }
//        else {
//            cerr << "Warning: Invalid line format: " << line << endl;
//        }
//    }
//
//    infile.close();
//
//    // Ensure starting and ending positions are not corrupted
//    if (grid[0][0] == '#') {
//        cout << "Starting position (0,0) is corrupted. No path possible." << endl;
//        return 0;
//    }
//    if (grid[70][70] == '#') {
//        cout << "Exit position (70,70) is corrupted. No path possible." << endl;
//        return 0;
//    }
//
//    // BFS to find the shortest path
//    // Each element in the queue is a pair of coordinates and the number of steps taken to reach there
//    queue<pair<pair<int, int>, int>> q;
//    vector<vector<bool>> visited(GRID_SIZE, vector<bool>(GRID_SIZE, false));
//
//    // Start from (0,0) with 0 steps
//    q.push({ {0, 0}, 0 });
//    visited[0][0] = true;
//
//    bool path_found = false;
//    int min_steps = -1;
//
//    while (!q.empty()) {
//        auto current = q.front();
//        q.pop();
//
//        int x = current.first.first;
//        int y = current.first.second;
//        int steps = current.second;
//
//        // Check if we've reached the exit
//        if (x == 70 && y == 70) {
//            min_steps = steps;
//            path_found = true;
//            break;
//        }
//
//        // Explore all possible directions
//        for (const auto& dir : DIRECTIONS) {
//            int new_x = x + dir.first;
//            int new_y = y + dir.second;
//
//            if (isValid(new_x, new_y) && !visited[new_y][new_x] && grid[new_y][new_x] != '#') {
//                visited[new_y][new_x] = true;
//                q.push({ {new_x, new_y}, steps + 1 });
//            }
//        }
//    }
//
//    if (path_found) {
//        cout << "Minimum number of steps to reach the exit: " << min_steps << endl;
//    }
//    else {
//        cout << "No path found to reach the exit." << endl;
//    }
//
//    return 0;
//}



#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
using namespace std;

// Define the size of the grid
const int GRID_SIZE = 71;

// Directions for movement: up, down, left, right
const int dx[4] = { 0, 0, -1, 1 };
const int dy[4] = { -1, 1, 0, 0 };

// Function to perform BFS and check if a path exists
bool isPathAvailable(const vector<vector<bool>>& grid) {
   // Check if start or exit is corrupted
   if (grid[0][0] || grid[GRID_SIZE - 1][GRID_SIZE - 1]) {
       return false;
   }

   // Initialize visited grid
   vector<vector<bool>> visited(GRID_SIZE, vector<bool>(GRID_SIZE, false));
   queue<pair<int, int>> q;

   // Start BFS from (0,0)
   q.push({ 0, 0 });
   visited[0][0] = true;

   while (!q.empty()) {
       auto current = q.front();
       q.pop();

       int x = current.first;
       int y = current.second;

       // Check if we've reached the exit
       if (x == GRID_SIZE - 1 && y == GRID_SIZE - 1) {
           return true;
       }

       // Explore all four directions
       for (int dir = 0; dir < 4; ++dir) {
           int newX = x + dx[dir];
           int newY = y + dy[dir];

           // Check bounds
           if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE) {
               // Check if the cell is not corrupted and not visited
               if (!grid[newX][newY] && !visited[newX][newY]) {
                   visited[newX][newY] = true;
                   q.push({ newX, newY });
               }
           }
       }
   }

   // If exit was not reached
   return false;
}

int main() {
   // Open the input file
   ifstream infile("input18.txt");
   if (!infile.is_open()) {
     cerr << "Error: Could not open input18.txt" << endl;
       return 1;
   }

   // Initialize the grid as uncorrupted
   vector<vector<bool>> grid(GRID_SIZE, vector<bool>(GRID_SIZE, false));

   string line;
   // Read each byte position
   while (getline(infile, line)) {
       // Skip empty lines
       if (line.empty()) continue;

       // Parse X and Y
       int X, Y;
       char comma;
       stringstream ss(line);
       ss >> X >> comma >> Y;

       // Validate coordinates
       if (X < 0 || X >= GRID_SIZE || Y < 0 || Y >= GRID_SIZE) {
           cerr << "Invalid coordinate: " << X << "," << Y << endl;
           continue;
       }

       // Mark the cell as corrupted
       grid[X][Y] = true;

       // Check if a path still exists
       if (!isPathAvailable(grid)) {
           // Output the coordinates in the required format
           cout << X << "," << Y << endl;
           return 0;
       }
   }

   // If all bytes processed and path still exists
   cout << "No byte found that blocks the path." << endl;
   return 0;
}
