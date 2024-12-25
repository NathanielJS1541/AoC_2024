////#include <iostream>
////#include <fstream>
////#include <string>
////#include <vector>
////#include <queue>
////#include <utility>
////
////using namespace std;
////
////struct Cell {
////    int x, y;
////};
////
////int main() {
////    ios::sync_with_stdio(false);
////    cin.tie(nullptr);
////
////    ifstream in("input20.txt");
////    if (!in.is_open()) {
////        cerr << "Could not open input20.txt\n";
////        return 1;
////    }
////
////    vector<string> grid;
////    {
////        string line;
////        while (getline(in, line)) {
////            grid.push_back(line);
////        }
////    }
////
////    int R = (int)grid.size();
////    int C = (int)grid[0].size();
////
////    int Sx = -1, Sy = -1, Ex = -1, Ey = -1;
////
////    // Find S and E
////    for (int i = 0; i < R; i++) {
////        for (int j = 0; j < C; j++) {
////            if (grid[i][j] == 'S') {
////                Sx = i; Sy = j;
////            }
////            else if (grid[i][j] == 'E') {
////                Ex = i; Ey = j;
////            }
////        }
////    }
////
////    int dx[4] = { -1,1,0,0 };
////    int dy[4] = { 0,0,-1,1 };
////
////    auto valid = [&](int x, int y) {
////        return x >= 0 && x < R && y >= 0 && y < C;
////        };
////
////    // BFS from S
////    vector<vector<int>> distFromS(R, vector<int>(C, -1));
////    {
////        queue<Cell> q;
////        distFromS[Sx][Sy] = 0;
////        q.push({ Sx,Sy });
////        while (!q.empty()) {
////            Cell cur = q.front(); q.pop();
////            int x = cur.x, y = cur.y;
////            for (int k = 0; k < 4; k++) {
////                int nx = x + dx[k], ny = y + dy[k];
////                if (valid(nx, ny) && grid[nx][ny] != '#' && distFromS[nx][ny] == -1) {
////                    distFromS[nx][ny] = distFromS[x][y] + 1;
////                    q.push({ nx,ny });
////                }
////            }
////        }
////    }
////
////    // BFS from E
////    vector<vector<int>> distFromE(R, vector<int>(C, -1));
////    {
////        queue<Cell> q;
////        distFromE[Ex][Ey] = 0;
////        q.push({ Ex,Ey });
////        while (!q.empty()) {
////            Cell cur = q.front(); q.pop();
////            int x = cur.x, y = cur.y;
////            for (int k = 0; k < 4; k++) {
////                int nx = x + dx[k], ny = y + dy[k];
////                if (valid(nx, ny) && grid[nx][ny] != '#' && distFromE[nx][ny] == -1) {
////                    distFromE[nx][ny] = distFromE[x][y] + 1;
////                    q.push({ nx,ny });
////                }
////            }
////        }
////    }
////
////    int baseline = distFromS[Ex][Ey];
////    if (baseline == -1) {
////        // No baseline path
////        cout << 0 << "\n";
////        return 0;
////    }
////
////    long long countCheats = 0;
////
////    for (int x = 0; x < R; x++) {
////        for (int y = 0; y < C; y++) {
////            // Must be track reachable from S
////            if (grid[x][y] == '#' || distFromS[x][y] == -1) continue;
////
////            for (int d1 = 0; d1 < 4; d1++) {
////                int x1 = x + dx[d1], y1 = y + dy[d1];
////                if (!valid(x1, y1)) continue;
////                // First move can pass through anything (cheat)
////                for (int d2 = 0; d2 < 4; d2++) {
////                    int x2 = x1 + dx[d2], y2 = y1 + dy[d2];
////                    if (!valid(x2, y2)) continue;
////                    // End cell must be track and reachable from E
////                    if (grid[x2][y2] == '#' || distFromE[x2][y2] == -1) continue;
////
////                    int possibleTime = distFromS[x][y] + 2 + distFromE[x2][y2];
////                    int time_saved = baseline - possibleTime;
////                    if (time_saved >= 100) {
////                        countCheats++;
////                    }
////                }
////            }
////        }
////    }
////
////    cout << countCheats << "\n";
////
////    return 0;
////}
//
//
//
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>
//#include <queue>
//#include <unordered_map>
//
//using namespace std;
//
//struct Cell {
//    int x, y;
//};
//
//static const int DX[4] = { -1,1,0,0 };
//static const int DY[4] = { 0,0,-1,1 };
//
//inline bool valid(int x, int y, int R, int C) {
//    return x >= 0 && x < R && y >= 0 && y < C;
//}
//
//// Encode a pair of cells into a single long long key
//static long long encodeKey(int x1, int y1, int x2, int y2) {
//    // We assume R,C < 65536 (common in AoC)
//    // Pack coordinates into 16 bits each:
//    long long key = 0;
//    key = ((long long)x1 << 48) | ((long long)y1 << 32) | ((long long)x2 << 16) | (long long)y2;
//    return key;
//}
//
//int main() {
//    ios::sync_with_stdio(false);
//    cin.tie(nullptr);
//
//    ifstream in("input20.txt");
//    if (!in.is_open()) {
//        cerr << "Cannot open input20.txt\n";
//        return 1;
//    }
//
//    vector<string> grid;
//    {
//        string line;
//        while (getline(in, line)) {
//            grid.push_back(line);
//        }
//    }
//
//    int R = (int)grid.size();
//    int C = (int)grid[0].size();
//
//    int Sx = -1, Sy = -1, Ex = -1, Ey = -1;
//    for (int i = 0; i < R; i++) {
//        for (int j = 0; j < C; j++) {
//            if (grid[i][j] == 'S') { Sx = i; Sy = j; }
//            else if (grid[i][j] == 'E') { Ex = i; Ey = j; }
//        }
//    }
//
//    // BFS from S
//    vector<vector<int>> distFromS(R, vector<int>(C, -1));
//    {
//        queue<Cell>q;
//        distFromS[Sx][Sy] = 0;
//        q.push({ Sx,Sy });
//        while (!q.empty()) {
//            auto [x, y] = q.front(); q.pop();
//            for (int k = 0; k < 4; k++) {
//                int nx = x + DX[k], ny = y + DY[k];
//                if (valid(nx, ny, R, C) && grid[nx][ny] != '#' && distFromS[nx][ny] == -1) {
//                    distFromS[nx][ny] = distFromS[x][y] + 1;
//                    q.push({ nx,ny });
//                }
//            }
//        }
//    }
//
//    // BFS from E
//    vector<vector<int>> distFromE(R, vector<int>(C, -1));
//    {
//        queue<Cell>q;
//        distFromE[Ex][Ey] = 0;
//        q.push({ Ex,Ey });
//        while (!q.empty()) {
//            auto [x, y] = q.front(); q.pop();
//            for (int k = 0; k < 4; k++) {
//                int nx = x + DX[k], ny = y + DY[k];
//                if (valid(nx, ny, R, C) && grid[nx][ny] != '#' && distFromE[nx][ny] == -1) {
//                    distFromE[nx][ny] = distFromE[x][y] + 1;
//                    q.push({ nx,ny });
//                }
//            }
//        }
//    }
//
//    int baseline = distFromS[Ex][Ey];
//    if (baseline == -1) {
//        // No baseline path, no cheats can save time since we don't have a baseline reference.
//        cout << 0 << "\n";
//        return 0;
//    }
//
//    // We will store minimal steps for each cheat identified by start and end cells
//    unordered_map<long long, int> cheatMap;
//    // key = encodeKey(Sx,Sy,Ex,Ey), value = minimal cheat steps
//
//    // For each cell A that is reachable from S
//    for (int Ax = 0; Ax < R; Ax++) {
//        for (int Ay = 0; Ay < C; Ay++) {
//            if (grid[Ax][Ay] == '#') continue;
//            if (distFromS[Ax][Ay] == -1) continue; // not reachable from S
//
//            // BFS ignoring walls from A to find all track cells B within 20 steps
//            vector<vector<int>> dist(R, vector<int>(C, -1));
//            queue<Cell>q;
//            dist[Ax][Ay] = 0;
//            q.push({ Ax,Ay });
//            while (!q.empty()) {
//                auto [x, y] = q.front(); q.pop();
//                int d = dist[x][y];
//                if (d == 20) continue; // can't go beyond 20 steps
//
//                for (int k = 0; k < 4; k++) {
//                    int nx = x + DX[k], ny = y + DY[k];
//                    if (!valid(nx, ny, R, C)) continue;
//                    // Ignoring walls, so no check on grid[nx][ny] for movement
//                    if (dist[nx][ny] == -1) {
//                        dist[nx][ny] = d + 1;
//                        q.push({ nx,ny });
//                    }
//                }
//            }
//
//            // Now dist[][] has minimal steps from A ignoring walls
//            // For each track cell B in dist:
//            for (int Bx = 0; Bx < R; Bx++) {
//                for (int By = 0; By < C; By++) {
//                    int steps = dist[Bx][By];
//                    if (steps == -1) continue; // not reachable
//                    if (grid[Bx][By] == '#') continue; // must end on track
//                    // steps <=20
//
//                    // distFromE[Bx][By] must be known
//                    if (distFromE[Bx][By] == -1) continue;
//
//                    // Record minimal steps for this cheat (A,B)
//                    long long key = encodeKey(Ax, Ay, Bx, By);
//                    auto it = cheatMap.find(key);
//                    if (it == cheatMap.end() || it->second > steps) {
//                        cheatMap[key] = steps;
//                    }
//                }
//            }
//        }
//    }
//
//    // Compute how many have time_saved >= 100
//    long long countCheats = 0;
//    for (auto& kv : cheatMap) {
//        int steps = kv.second;
//        // decode key?
//        long long key = kv.first;
//        int Ax = (int)((key >> 48) & 0xFFFF);
//        int Ay = (int)((key >> 32) & 0xFFFF);
//        int Bx = (int)((key >> 16) & 0xFFFF);
//        int By = (int)(key & 0xFFFF);
//
//        // Compute time_saved
//        int possibleTime = distFromS[Ax][Ay] + steps + distFromE[Bx][By];
//        int time_saved = baseline - possibleTime;
//        if (time_saved >= 100) countCheats++;
//    }
//
//    cout << countCheats << "\n";
//    return 0;
//}





// ------------------------------------------------------
// Visualization
//
//------------------------------------------------------


#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>

// ANSI color codes
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define GRAY    "\033[90m"

using namespace std;

struct Cell {
   int x, y;
};

int main() {
   ifstream in("input20.txt");
   if (!in) {
       cerr << "Could not open input20.txt\n";
       return 1;
   }

   vector<string> grid;
   {
       string line;
       while (getline(in, line)) {
           grid.push_back(line);
       }
   }

   int R = (int)grid.size();
   int C = (int)grid[0].size();

   int Sx = -1, Sy = -1, Ex = -1, Ey = -1;

   // Locate S and E
   for (int i = 0; i < R; i++) {
       for (int j = 0; j < C; j++) {
           if (grid[i][j] == 'S') { Sx = i; Sy = j; }
           if (grid[i][j] == 'E') { Ex = i; Ey = j; }
       }
   }

   // Directions for BFS
   int dx[4] = { -1,1,0,0 };
   int dy[4] = { 0,0,-1,1 };

   auto valid = [&](int x, int y) {
       return x >= 0 && x < R && y >= 0 && y < C;
       };

   // BFS for shortest path (no cheat)
   vector<vector<int>> dist(R, vector<int>(C, -1));
   vector<vector<pair<int, int>>> parent(R, vector<pair<int, int>>(C, { -1,-1 }));
   queue<Cell>q;
   dist[Sx][Sy] = 0;
   q.push({ Sx,Sy });

   bool found = false;
   while (!q.empty()) {
       auto [x, y] = q.front(); q.pop();
       if (x == Ex && y == Ey) {
           found = true;
           break;
       }
       for (int k = 0; k < 4; k++) {
           int nx = x + dx[k], ny = y + dy[k];
           if (valid(nx, ny) && grid[nx][ny] != '#' && dist[nx][ny] == -1) {
               dist[nx][ny] = dist[x][y] + 1;
               parent[nx][ny] = { x,y };
               q.push({ nx,ny });
           }
       }
   }

   if (!found) {
       cout << "No path found!\n";
       return 0;
   }

   // Reconstruct path
   vector<pair<int, int>> path;
   {
       int cx = Ex, cy = Ey;
       while (!(cx == Sx && cy == Sy)) {
           path.push_back({ cx,cy });
           auto p = parent[cx][cy];
           cx = p.first; cy = p.second;
       }
       path.push_back({ Sx,Sy });
       reverse(path.begin(), path.end());
   }

   // For demonstration, let's assume we "cheated" on a portion of this path.
   // Suppose we activate cheat at path index 5 and cheat for 3 steps.
   // In a real solution, you'd determine these cells from your logic.
   int cheatStartIndex = 5;
   int cheatLength = 3;
   // Make sure cheat doesn't exceed path length
   cheatLength = min((int)path.size() - cheatStartIndex, cheatLength);

   // Mark path steps:
   // - Normal path: 'o' (Cyan)
   // - Cheat path: '*' (Yellow)
   vector<string> visual = grid;
   for (int i = 0; i < (int)path.size(); i++) {
       int x = path[i].first, y = path[i].second;
       if (visual[x][y] == '.') {
           if (i >= cheatStartIndex && i < cheatStartIndex + cheatLength) {
               // cheat cell
               visual[x][y] = '*';
           }
           else {
               // normal path cell
               visual[x][y] = 'o';
           }
       }
   }
   // Ensure S and E remain S and E
   visual[Sx][Sy] = 'S';
   visual[Ex][Ey] = 'E';

   // Print the visual with colors
   // Walls (#): Bold Red
   // Track (.): Gray dot
   // Start (S), End (E): Bold Green
   // Normal path (o): Cyan
   // Cheat path (*): Yellow
   for (int i = 0; i < R; i++) {
       for (int j = 0; j < C; j++) {
           char c = visual[i][j];
           if (c == '#') {
               cout << BOLD << RED << "#" << RESET;
           }
           else if (c == 'S' || c == 'E') {
               cout << BOLD << GREEN << c << RESET;
           }
           else if (c == 'o') {
               cout << CYAN << "o" << RESET;
           }
           else if (c == '*') {
               cout << YELLOW << "*" << RESET;
           }
           else if (c == '.') {
               cout << GRAY << "." << RESET;
           }
           else {
               // Any other character (rare): just print default
               cout << c;
           }
       }
       cout << "\n";
   }

   return 0;
}
