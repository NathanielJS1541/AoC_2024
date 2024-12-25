#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//--------------------------------------------------------------------------------------
//                             Direction Setup                                          //
// Directions: 0 = Up, 1 = Right, 2 = Down, 3 = Left                                    //
// These arrays help determine the guard's next move based on current direction.        //
//--------------------------------------------------------------------------------------
const int dx[] = { 0, 1, 0, -1 };
const int dy[] = { -1, 0, 1, 0 }; 

//-------------------------------------------------------------------------------------------
//               Function to Locate Guard's Starting Position and Direction                 //
// This function scans the grid to find where the guard starts and which way they're facing.//
// It looks for symbols: '^', '>', 'v', '<' indicating Up, Right, Down, Left respectively.  //
//--------------------------------------------------------------------------------------
bool findInitialPosition(const vector<string>& grid, int& start_x, int& start_y, int& start_dir) {
   int height = grid.size();
   if (height == 0) return false; 
   int width = grid[0].size();

   
   for (int y = 0; y < height; ++y) {
       for (int x = 0; x < width; ++x) {
           char c = grid[y][x];
           switch (c) {
           case '^':
               start_x = x;
               start_y = y;
               start_dir = 0; 
               return true;
           case '>':
               start_x = x;
               start_y = y;
               start_dir = 1;
               return true;
           case 'v':
               start_x = x;
               start_y = y;
               start_dir = 2; 
               return true;
           case '<':
               start_x = x;
               start_y = y;
               start_dir = 3; 
               return true;
           default:
               break; 
           }
       }
   }
   return false; 
}

//--------------------------------------------------------------------------------------
//                           Function to Simulate Guard's Movement                  //
// This function simulates how the guard moves through the grid.
// It returns the number of unique positions the guard visits before exiting or looping.
// If 'detect_loop' is true, it returns -1 if a loop is detected.
//--------------------------------------------------------------------------------------
int simulateGuard(vector<string>& grid, int start_x, int start_y, int start_dir, bool detect_loop = false) {
   int height = grid.size();
   int width = grid[0].size();
   //----------------------------------------------------------------------//
   // Create a unique identifier for each state (position and direction)   //
   // This helps in detecting loops by checking if the same state is revisited//
   //----------------------------------------------------------------------//
   vector<int> state_step(height * width * 4, -1);
   int step = 0; 

   
   int x = start_x;
   int y = start_y;
   int dir = start_dir;

   int initial_state = ((y * width + x) * 4) + dir;
   state_step[initial_state] = step;

   int distinct_count = 1; 

   vector<bool> visited(height * width, false);
   visited[y * width + x] = true; 

   while (true) {
    
       int nx = x + dx[dir];
       int ny = y + dy[dir];

     
       if (nx < 0 || ny < 0 || nx >= width || ny >= height) {
           break;
       }

    
       if (grid[ny][nx] == '#') {
           dir = (dir + 1) % 4;
          
           int new_state = ((y * width + x) * 4) + dir;
           step++;
           if (detect_loop) {
              
               if (state_step[new_state] != -1) {
                   return -1; 
               }
               state_step[new_state] = step; 
           }
           continue; 
       }

      
       x = nx;
       y = ny;
       step++;

      
       int current_state = ((y * width + x) * 4) + dir;
       if (detect_loop) {
         
           if (state_step[current_state] != -1) {
               return -1; 
           }
           state_step[current_state] = step; 
       }

    
       int pos_id = y * width + x;
       if (!visited[pos_id]) {
           visited[pos_id] = true; 
           distinct_count++; 
       }
   }

   return distinct_count; // Return the total unique positions visited
}

int main() {
   //----------------------------------------------------------------------------------
   //                      Optimize Input/Output Operations
   // This makes the program run faster by untieing cin from cout and disabling synchronization.
   //----------------------------------------------------------------------------------
   ios::sync_with_stdio(false);
   cin.tie(NULL);

  
   ifstream file("input06.txt");
   if (!file.is_open()) {
       cerr << "Error: Could not open the file 'input06.txt'." << endl;
       return 1;
   }

   vector<string> grid;
   string line;
   while (getline(file, line)) {
       if (!line.empty()) { 
           grid.push_back(line);
       }
   }
   file.close();

   //----------------------------------------------------------------------------------
   //                             Validating the Grid
   // Ensure that the grid isn't empty to avoid processing errors.
   //----------------------------------------------------------------------------------
   if (grid.empty()) {
       cerr << "Error: The grid is empty." << endl;
       return 1;
   }

   int height = grid.size();
   int width = grid[0].size();

   //----------------------------------------------------------------------------------
   //                 Finding Guard's Starting Position and Direction
   // Locate where the guard is and which way they're facing in the grid.
   //----------------------------------------------------------------------------------
   int start_x = 0, start_y = 0, start_dir = 0;
   if (!findInitialPosition(grid, start_x, start_y, start_dir)) {
       cerr << "Error: Guard's initial position not found in the grid." << endl;
       return 1;
   }

   //----------------------------------------------------------------------------------
   //                     Part One: Simulate Guard's Movement
   // Calculate how many unique positions the guard visits without any new obstructions.
   //----------------------------------------------------------------------------------
   int distinct_positions = simulateGuard(grid, start_x, start_y, start_dir, false);
   cout << "Part One - Distinct positions visited: " << distinct_positions << endl;

   //--------------------------------------------------------------------------------------
   //            Part Two: Find Obstruction Positions Causing Infinite Loops
   // Determine how many places you can add a single obstacle to trap the guard in a loop.
   //--------------------------------------------------------------------------------------
   int loop_count = 0;

   // Go through every cell in the grid to test placing a new obstruction
   for (int y = 0; y < height; ++y) {
       for (int x = 0; x < width; ++x) {
           // Skip cells that are already obstacles or the guard's starting point
           if (grid[y][x] == '#' || (x == start_x && y == start_y)) {
               continue;
           }

           // Temporarily place an obstruction in the current cell
           grid[y][x] = '#';

           // Simulate the guard's movement with the new obstruction and check for loops
           int result = simulateGuard(grid, start_x, start_y, start_dir, true);

           // If a loop is detected, count this obstruction placement as valid
           if (result == -1) {
               loop_count++;
           }

           // Remove the temporary obstruction to reset the grid for the next test
           grid[y][x] = '.';
       }
   }

   // Display the total number of obstruction positions that cause the guard to loop
   cout << "Part Two - Possible obstruction positions causing a loop: " << loop_count << endl;

   return 0;
}
