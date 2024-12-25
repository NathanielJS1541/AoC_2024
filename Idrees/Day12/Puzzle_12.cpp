
//#include <iostream>
//#include <vector>
//#include <queue>
//#include <fstream>
//#include <unordered_set>
//#include <unordered_map>
//#include <string>
//
//using namespace std;
//using ll = long long;
//using pii = pair<int, int>;
//
//// Custom hash function for pairs
//struct pair_hash {
//    template <class T1, class T2>
//    std::size_t operator() (const std::pair<T1, T2>& pair) const {
//        auto hash1 = std::hash<T1>{}(pair.first);
//        auto hash2 = std::hash<T2>{}(pair.second);
//        return hash1 ^ hash2;
//    }
//};
//
//// Define Region using custom hash for pair
//using Region = unordered_set<pii, pair_hash>;
//
//const int dx[] = { -1, 0, 1, 0 }; // Directions for navigating through the grid (up, right, down, left)
//const int dy[] = { 0, 1, 0, -1 };
//
//vector<string> readGrid(const string& filename) {
//    ifstream infile(filename);
//    vector<string> grid;
//    string line;
//
//    if (!infile.is_open()) {
//        cerr << "Error: Could not open the file." << endl;
//        return grid;
//    }
//
//    while (getline(infile, line)) {
//        if (!line.empty() && line.back() == '\r') {
//            line.pop_back();
//        }
//        grid.push_back(line);
//    }
//
//    infile.close();
//    return grid;
//}
//
//bool inBounds(int x, int y, int n, int m) {
//    return x >= 0 && y >= 0 && x < n && y < m;
//}
//
//Region findRegion(int sx, int sy, const vector<string>& grid) {
//    int n = grid.size(), m = grid[0].size();
//    vector<vector<bool>> visited(n, vector<bool>(m, false));
//    Region region;
//    queue<pii> q;
//
//    q.push({ sx, sy });
//    visited[sx][sy] = true;
//    region.insert({ sx, sy });
//
//    while (!q.empty()) {
//        auto [x, y] = q.front(); q.pop();
//        for (int dir = 0; dir < 4; ++dir) {
//            int nx = x + dx[dir], ny = y + dy[dir];
//            if (inBounds(nx, ny, n, m) && !visited[nx][ny] && grid[nx][ny] == grid[x][y]) {
//                visited[nx][ny] = true;
//                q.push({ nx, ny });
//                region.insert({ nx, ny });
//            }
//        }
//    }
//
//    return region;
//}
//
//// Function to calculate number of contiguous fence sides for a given region
//int calculateSides(const Region& region, const vector<string>& grid) {
//    int n = grid.size(), m = grid[0].size();
//    int sides = 0;
//
//    for (auto [x, y] : region) {
//        for (int dir = 0; dir < 4; ++dir) {
//            int nx = x + dx[dir], ny = y + dy[dir];
//            // Check if the adjacent cell is out of bounds or not part of the region
//            if (!inBounds(nx, ny, n, m) || region.find({ nx, ny }) == region.end()) {
//                sides++;
//            }
//        }
//    }
//
//    return sides;
//}
//
//int main() {
//    string filename = "input12.txt";
//    vector<string> grid = readGrid(filename);
//    if (grid.empty()) return 0;
//
//    int n = grid.size(), m = grid[0].size();
//    vector<vector<bool>> visited(n, vector<bool>(m, false));
//    ll totalFenceCost = 0;
//
//    for (int i = 0; i < n; ++i) {
//        for (int j = 0; j < m; ++j) {
//            if (!visited[i][j]) {
//                Region region = findRegion(i, j, grid);
//                for (auto [x, y] : region) visited[x][y] = true;
//
//                int area = region.size();
//                int sides = calculateSides(region, grid);
//                totalFenceCost += (ll)area * (ll)sides;
//            }
//        }
//    }
//
//    cout << "Total fence cost (Part Two - Number of Sides): " << totalFenceCost << endl;
//
//    return 0;
//}




#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include<sstream>

using namespace std;

struct Edge {
   int x, y, d;
   bool operator<(const Edge& other) const {
       return tie(x, y, d) < tie(other.x, other.y, other.d);
   }
};

vector<string> readGrid(const string& filename) {
   ifstream file(filename);
   vector<string> grid;
   string line;
   while (getline(file, line)) {
       if (!line.empty()) {
           grid.push_back(line);
       }
   }
   file.close();
   return grid;
}

int main() {
   vector<string> grid = readGrid("input12.txt");
   int rows = grid.size();
   int cols = grid[0].size();

   vector<vector<bool>> seen(rows, vector<bool>(cols, false));
   int result = 0;

   for (int sy = 0; sy < rows; ++sy) {
       for (int sx = 0; sx < cols; ++sx) {
           if (seen[sy][sx]) continue;

           queue<pair<int, int>> q;
           q.push({ sx, sy });
           int area = 0;
           set<Edge> sides;

           while (!q.empty()) {
               auto [x, y] = q.front();
               q.pop();

               if (seen[y][x]) continue;
               seen[y][x] = true;
               area++;

               static const int dx[] = { 1, -1, 0, 0 };
               static const int dy[] = { 0, 0, 1, -1 };

               for (int d = 0; d < 4; ++d) {
                   int nx = x + dx[d];
                   int ny = y + dy[d];

                   if (nx < 0 || nx >= cols || ny < 0 || ny >= rows || grid[ny][nx] != grid[sy][sx]) {
                       sides.insert({ nx, ny, d });
                   }
                   else {
                       q.push({ nx, ny });
                   }
               }
           }

           set<Edge> uniqueSides;
           for (const auto& s : sides) {
               if (sides.find({ s.x - (s.d >= 2), s.y - (s.d < 2), s.d }) == sides.end()) {
                   uniqueSides.insert(s);
               }
           }

           result += area * uniqueSides.size();
       }
   }

   cout << "Total price (Part Two): " << result << endl;
   return 0;
}
