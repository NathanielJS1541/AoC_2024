#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

int main() {
   cin.tie(0)->sync_with_stdio(0);

   // File handling
   ifstream input_file("input14.txt");  // Input file for robot positions
   ofstream output_file("output.txt"); // Output file for results

   if (!input_file.is_open() || !output_file.is_open()) {
       cerr << "Error opening file!" << endl;
       return 1;
   }

   string line;
   int N = 103, M = 101;

   // Struct to represent a Robot with position and velocity
   struct Robot {
       int px, py, vx, vy;
   };

   // 3D vector to store robots' information based on their positions
   vector<vector<vector<Robot>>> g(N, vector<vector<Robot>>(M));

   // Reading the input data from the file
   while (getline(input_file, line)) {
       istringstream iss(line);
       char c;
       int px, py;
       int vx, vy;
       iss >> c >> c >> py >> c >> px >> c >> c >> vy >> c >> vx;
       g[px][py].push_back({ px, py, vx, vy });
   }

   // Function to print the current state of the grid
   auto print = [&](int it) {
       output_file << it << '\n';
       for (int i = 0; i < N; ++i) {
           for (int j = 0; j < M; ++j) {
               output_file << (g[i][j].size() != 0 ? '#' : ' ');
           }
           output_file << '\n';
       }
       };

   int its = 10000;  // The number of iterations to run
   for (int it = 0; it < its; ++it) {
       int clustering = 0;
       for (int i = 1; i < N - 1; ++i) {
           for (int j = 1; j < M - 1; ++j) {
               if (g[i][j].size()) {
                   clustering += g[i - 1][j].size();
                   clustering += g[i][j - 1].size();
                   clustering += g[i + 1][j].size();
                   clustering += g[i][j + 1].size();
               }
           }
       }
       if (clustering >= 1000) {
           print(it);
       }

       // Create the next state of the grid
       vector<vector<vector<Robot>>> nxt(N, vector<vector<Robot>>(M));

       for (int i = 0; i < N; ++i) {
           for (int j = 0; j < M; ++j) {
               for (auto [px, py, vx, vy] : g[i][j]) {
                   int nx = i + vx, ny = j + vy;
                   if (nx < 0) nx += N;
                   if (ny < 0) ny += M;
                   nx %= N, ny %= M;
                   nxt[nx][ny].push_back({ px, py, vx, vy });
               }
           }
       }

       // Update the grid with the new state
       g = nxt;
   }

   // Close the input and output files
   input_file.close();
   output_file.close();

   return 0;
}
