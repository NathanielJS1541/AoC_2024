//-------------------------------------------------------------
//   Puzzle 01
//------------------------------------------------------------



//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <string>
//using namespace std;
//bool isSafe(const vector<int>& levels) {
//    if (levels.size() < 2) return false; // Cannot determine from a single level
//
//    bool increasing = true;
//    bool decreasing = true;
//
//    for (size_t i = 1; i < levels.size(); ++i) {
//        int diff = levels[i] - levels[i - 1];
//        if (diff < 1 || diff > 3) increasing = false;
//        if (-diff < 1 || -diff > 3) decreasing = false;
//    }
//
//    return (increasing || decreasing) && levels.size() > 1; // Ensure at least two elements
//}
//
//int countSafeReports(const string& filename) {
//    ifstream inputFile(filename);
//    string line;
//    int safeCount = 0;
//
//    if (!inputFile.is_open()) {
//        cerr << "Error opening file: " << filename << endl;
//        return -1; // Return -1 to indicate error
//    }
//
//    while (getline(inputFile, line)) {
//        istringstream iss(line);
//        vector<int> levels;
//        int level;
//
//        while (iss >> level) {
//            levels.push_back(level);
//        }
//
//        if (isSafe(levels)) {
//            safeCount++;
//        }
//    }
//
//    inputFile.close();
//    return safeCount;
//}
//
//int main() {
//    string filename = "input01.txt"; // specify the input file name
//    int numberOfSafeReports = countSafeReports(filename);
//    if (numberOfSafeReports >= 0) {
//        cout << "Number of safe reports: " << numberOfSafeReports << endl;
//    }
//    return 0;
//}


//-------------------------------------------------------------
//   Puzzle 02
//------------------------------------------------------------


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>  

using namespace std;

bool isSafe(const vector<int>& nums) {
   if (nums.size() < 2) return true;

   int diff = nums[1] - nums[0];
   if (abs(diff) > 3 || abs(diff) < 1) return false;

   bool increasing = diff > 0;

   for (size_t i = 1; i < nums.size() - 1; ++i) {
       int curr_diff = nums[i + 1] - nums[i];
       if (abs(curr_diff) > 3 || abs(curr_diff) < 1) return false;
       if ((curr_diff > 0) != increasing) return false;
   }

   return true;
}

int main() {
   ifstream inputFile("input02.txt");
   string line;
   int safeCount = 0;

   if (!inputFile.is_open()) {
       cerr << "Error opening file" << endl;
       return 1;
   }

   while (getline(inputFile, line)) {
       istringstream iss(line);
       vector<int> nums;
       int num;

       while (iss >> num) {
           nums.push_back(num);
       }

       if (isSafe(nums)) {
           safeCount++;
       }
       else {
           for (size_t i = 0; i < nums.size(); ++i) {
               vector<int> test(nums);
               test.erase(test.begin() + i);
               if (isSafe(test)) {
                   safeCount++;
                   break;
               }
           }
       }
   }

   inputFile.close();
   cout << "Number of safe reports: " << safeCount << endl;

   return 0;
}
