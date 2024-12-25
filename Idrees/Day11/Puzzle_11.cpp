#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <utility>

using namespace std;

// Define a type for the memoization key
typedef pair<long long, int> MemoKey;

// Custom hash function for the unordered_map to handle pair as key
struct pair_hash {
   size_t operator()(const MemoKey& k) const {
       // A simple hash combination for pair<long long, int>
       return hash<long long>()(k.first) ^ (hash<int>()(k.second) << 16);
   }
};

// Memoization map to cache results
unordered_map<MemoKey, long long, pair_hash> memo;

// Function to split a number with even digits into two stones
pair<long long, long long> splitNumber(long long num) {
   string numStr = to_string(num);
   int len = numStr.length();
   int half = len / 2;
   string leftStr = numStr.substr(0, half);
   string rightStr = numStr.substr(half, len - half);

   // Remove leading zeros by converting back to integer
   long long left = leftStr.empty() ? 0 : stoll(leftStr);
   long long right = rightStr.empty() ? 0 : stoll(rightStr);

   return { left, right };
}

// Recursive function with memoization to count stones
long long countStones(long long stone, int blinks) {
   // Base case: no more blinks, each stone counts as one
   if (blinks == 0) {
       return 1;
   }

   // Create the memoization key
   MemoKey key = { stone, blinks };

   // Check if result is already computed
   auto it = memo.find(key);
   if (it != memo.end()) {
       return it->second;
   }

   long long total = 0;

   if (stone == 0) {
       // Rule 1: Replace 0 with 1
       total += countStones(1, blinks - 1);
   }
   else {
       // Determine the number of digits in the stone's number
       int numDigits = 0;
       long long temp = stone;
       if (temp == 0) {
           numDigits = 1;
       }
       else {
           while (temp > 0) {
               temp /= 10;
               numDigits++;
           }
       }

       if (numDigits % 2 == 0) {
           // Rule 2: Split into two stones
           pair<long long, long long> split = splitNumber(stone);
           total += countStones(split.first, blinks - 1);
           total += countStones(split.second, blinks - 1);
       }
       else {
           // Rule 3: Multiply by 2024
           long long newStone = stone * 2024;
           total += countStones(newStone, blinks - 1);
       }
   }

   // Store the result in the memoization map
   memo[key] = total;
   return total;
}

int main() {
   // Open the input file
   ifstream inputFile("input011.txt");
   if (!inputFile) {
       cerr << "Error: Unable to open input file 'input011.txt'. Please ensure the file exists in the executable directory.\n";
       return 1;
   }

   vector<long long> stones;
   string line;

   // Read the first line from the file and parse the initial stones
   if (getline(inputFile, line)) {
       stringstream ss(line);
       long long num;
       while (ss >> num) {
           stones.push_back(num);
       }
   }
   inputFile.close();

   if (stones.empty()) {
       cerr << "Error: No stones found in the input file.\n";
       return 1;
   }

   // Define the number of blinks for Part One and Part Two
   const int blinksPartOne = 25;  // Part One: 25 blinks
   const int blinksPartTwo = 75;  // Part Two: 75 blinks

   // Compute the total number of stones after Part One blinks
   long long finalCountPartOne = 0;
   for (auto stone : stones) {
       finalCountPartOne += countStones(stone, blinksPartOne);
   }

   cout << "Part One:\n";
   cout << "Number of stones after " << blinksPartOne << " blinks: " << finalCountPartOne << "\n\n";

   // Compute the total number of stones after Part Two blinks
   // Note: Assuming 75 blinks total, not 75 additional blinks
   long long finalCountPartTwo = 0;
   for (auto stone : stones) {
       finalCountPartTwo += countStones(stone, blinksPartTwo);
   }

   cout << "Part Two:\n";
   cout << "Number of stones after " << blinksPartTwo << " blinks: " << finalCountPartTwo << "\n";

   return 0;
}