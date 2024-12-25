#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Define a structure to represent coordinates
struct Coordinate {
   int x;
   int y;
};

// Function to check if two coordinates are collinear with a third
bool areCollinear(const Coordinate& a, const Coordinate& b, const Coordinate& c) {
   // Calculate the area of the triangle formed by the three points
   // If the area is zero, they are collinear
   return ((b.x - a.x) * (c.y - a.y)) == ((c.x - a.x) * (b.y - a.y));
}

int main() {
   // Open the input file
   ifstream infile("input08.txt");
   if (!infile) {
       cout << "Error: Cannot open input.txt" << endl;
       return 1;
   }

   // Read the input map into a grid
   vector<string> grid;
   string line;
   while (getline(infile, line)) {
       grid.push_back(line);
   }
   infile.close();

   // Check if the grid is empty
   if (grid.empty()) {
       cout << "Error: Input map is empty." << endl;
       return 1;
   }

   int numRows = grid.size();
   int numCols = grid[0].size();

   // Initialize an array to hold antennas for each possible ASCII character
   // Assuming frequencies are standard ASCII characters
   vector< vector<Coordinate> > freqGroups(256, vector<Coordinate>());

   // Traverse the grid to identify antennas and group them by frequency
   for (int y = 0; y < numRows; y++) {
       for (int x = 0; x < numCols; x++) {
           char cell = grid[y][x];
           if (cell != '.') {
               Coordinate antenna;
               antenna.x = x;
               antenna.y = y;
               freqGroups[(unsigned char)cell].push_back(antenna);
           }
       }
   }

   // Part One: Original Criteria
   // Initialize a 2D grid to mark antinode positions
   // false indicates no antinode, true indicates an antinode is present
   vector< vector<bool> > antinodeGridPartOne(numRows, vector<bool>(numCols, false));

   // Iterate through each frequency group for Part One
   for (int freq = 0; freq < 256; freq++) {
       // Proceed only if there are at least two antennas of this frequency
       if (freqGroups[freq].size() < 2)
           continue;

       // Get the number of antennas in this frequency group
       int numAntennas = freqGroups[freq].size();

       // Iterate through all unique pairs of antennas
       for (int i = 0; i < numAntennas; i++) {
           for (int j = i + 1; j < numAntennas; j++) {
               Coordinate A = freqGroups[freq][i];
               Coordinate B = freqGroups[freq][j];

               // Calculate Antinode C: Extending from A
               int Cx = 2 * B.x - A.x;
               int Cy = 2 * B.y - A.y;

               // Calculate Antinode D: Extending from B
               int Dx = 2 * A.x - B.x;
               int Dy = 2 * A.y - B.y;

               // Function to check if coordinates are within bounds
               auto isWithinBounds = [&](int x, int y) -> bool {
                   return (x >= 0 && x < numCols && y >= 0 && y < numRows);
                   };

               // Mark Antinode C if within bounds
               if (isWithinBounds(Cx, Cy)) {
                   antinodeGridPartOne[Cy][Cx] = true;
               }

               // Mark Antinode D if within bounds
               if (isWithinBounds(Dx, Dy)) {
                   antinodeGridPartOne[Dy][Dx] = true;
               }
           }
       }
   }

   // Count the number of unique antinode locations for Part One
   int antinodeCountPartOne = 0;
   for (int y = 0; y < numRows; y++) {
       for (int x = 0; x < numCols; x++) {
           if (antinodeGridPartOne[y][x])
               antinodeCountPartOne++;
       }
   }

   // Part Two: Updated Criteria
   // Initialize a 2D grid to mark antinode positions
   vector< vector<bool> > antinodeGridPartTwo(numRows, vector<bool>(numCols, false));

   // Iterate through each frequency group for Part Two
   for (int freq = 0; freq < 256; freq++) {
       // Proceed only if there are at least two antennas of this frequency
       if (freqGroups[freq].size() < 2)
           continue;

       // Get the number of antennas in this frequency group
       int numAntennas = freqGroups[freq].size();

       // Iterate through all possible triplets to find collinear points
       for (int i = 0; i < numAntennas; i++) {
           for (int j = i + 1; j < numAntennas; j++) {
               for (int k = j + 1; k < numAntennas; k++) {
                   Coordinate A = freqGroups[freq][i];
                   Coordinate B = freqGroups[freq][j];
                   Coordinate C = freqGroups[freq][k];

                   // Check if they are collinear
                   if (areCollinear(A, B, C)) {
                       // Mark the positions as antinodes
                       antinodeGridPartTwo[A.y][A.x] = true;
                       antinodeGridPartTwo[B.y][B.x] = true;
                       antinodeGridPartTwo[C.y][C.x] = true;
                   }
               }
           }

           // Additionally, find all positions in line with any two antennas
           for (int j = 0; j < numAntennas; j++) {
               if (j == i) continue;
               for (int k = j + 1; k < numAntennas; k++) {
                   if (k == i) continue;
                   Coordinate A = freqGroups[freq][i];
                   Coordinate B = freqGroups[freq][j];
                   Coordinate C = freqGroups[freq][k];

                   if (areCollinear(A, B, C)) {
                       // Any point in line with at least two antennas is an antinode
                       antinodeGridPartTwo[A.y][A.x] = true;
                       antinodeGridPartTwo[B.y][B.x] = true;
                       antinodeGridPartTwo[C.y][C.x] = true;
                   }
               }
           }
       }

       // Alternatively, a more efficient way is to consider all pairs and mark all points in line
       // However, for simplicity and given typical input sizes, the above method suffices
   }

   // Additionally, according to the updated criteria, any grid position exactly in line with at least two antennas
   // So, we need to consider all pairs and mark all points that are collinear with at least two antennas

   // To handle this more comprehensively, we'll iterate through all pairs and mark the grid positions
   // that lie on the line defined by each pair

   // Reset the antinodeGridPartTwo
   fill(antinodeGridPartTwo.begin(), antinodeGridPartTwo.end(), vector<bool>(numCols, false));

   for (int freq = 0; freq < 256; freq++) {
       if (freqGroups[freq].size() < 2)
           continue;

       int numAntennas = freqGroups[freq].size();

       // Iterate through all unique pairs
       for (int i = 0; i < numAntennas; i++) {
           for (int j = i + 1; j < numAntennas; j++) {
               Coordinate A = freqGroups[freq][i];
               Coordinate B = freqGroups[freq][j];

               // Determine the direction vector from A to B
               int dx = B.x - A.x;
               int dy = B.y - A.y;

               // Normalize the direction to step through the grid
               int gcd_val = 1;
               if (dx != 0 || dy != 0) {
                   // Compute the greatest common divisor
                   int a = abs(dx);
                   int b = abs(dy);
                   while (b != 0) {
                       int temp = b;
                       b = a % b;
                       a = temp;
                   }
                   gcd_val = a;
               }

               // Step increments
               int step_x = (dx == 0) ? 0 : dx / gcd_val;
               int step_y = (dy == 0) ? 0 : dy / gcd_val;

               // Iterate from A to B and beyond to cover the entire line
               // Starting from A, extend in the direction of B
               int x = A.x;
               int y = A.y;
               while (x >= 0 && x < numCols && y >= 0 && y < numRows) {
                   antinodeGridPartTwo[y][x] = true;
                   x += step_x;
                   y += step_y;
               }

               // Starting from A, extend in the opposite direction of B
               x = A.x - step_x;
               y = A.y - step_y;
               while (x >= 0 && x < numCols && y >= 0 && y < numRows) {
                   antinodeGridPartTwo[y][x] = true;
                   x -= step_x;
                   y -= step_y;
               }

               // Similarly, iterate from B to A and beyond
               x = B.x;
               y = B.y;
               while (x >= 0 && x < numCols && y >= 0 && y < numRows) {
                   antinodeGridPartTwo[y][x] = true;
                   x += (-step_x);
                   y += (-step_y);
               }

               // Starting from B, extend in the opposite direction of A
               x = B.x + step_x;
               y = B.y + step_y;
               while (x >= 0 && x < numCols && y >= 0 && y < numRows) {
                   antinodeGridPartTwo[y][x] = true;
                   x += step_x;
                   y += step_y;
               }
           }
       }
   }

   // Additionally, mark all antenna positions as antinodes if there are at least two antennas of the same frequency
   for (int freq = 0; freq < 256; freq++) {
       if (freqGroups[freq].size() < 2)
           continue;

       for (int i = 0; i < freqGroups[freq].size(); i++) {
           Coordinate antenna = freqGroups[freq][i];
           antinodeGridPartTwo[antenna.y][antenna.x] = true;
       }
   }

   // Count the number of unique antinode locations for Part Two
   int antinodeCountPartTwo = 0;
   for (int y = 0; y < numRows; y++) {
       for (int x = 0; x < numCols; x++) {
           if (antinodeGridPartTwo[y][x])
               antinodeCountPartTwo++;
       }
   }

   // Output the results
   cout << "Part One:" << endl;
   cout << "Number of unique antinode locations: " << antinodeCountPartOne << endl;
   cout << "Part Two:" << endl;
   cout << "Number of unique antinode locations: " << antinodeCountPartTwo << endl;

   return 0;
}
