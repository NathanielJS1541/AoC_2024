#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <map>
#include <cmath> // for std::abs

using namespace std;

vector<string> split(const string& s, const string& delimiter) {
   size_t pos_start = 0, pos_end, delim_len = delimiter.length();
   string token;
   vector<string> res;

   while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
       token = s.substr(pos_start, pos_end - pos_start);
       pos_start = pos_end + delim_len;
       res.push_back(token);
   }

   res.push_back(s.substr(pos_start));
   return res;
}

int calculateTotalDistance(const vector<int>& a, const vector<int>& b) {
   int ans = 0;
   for (size_t i = 0; i < a.size(); i++) {
       ans += abs(a[i] - b[i]);
   }
   return ans;
}

int calculateSimilarityScore(const vector<int>& a, const vector<int>& b) {
   map<int, int> frequencyMap;
   for (int num : b) {
       frequencyMap[num]++;
   }

   int similarityScore = 0;
   for (int num : a) {
       if (frequencyMap.find(num) != frequencyMap.end()) {
           similarityScore += num * frequencyMap[num];
       }
   }
   return similarityScore;
}

int main() {
   ifstream inputFile("input.txt");
   string line;
   vector<int> a, b;

   if (!inputFile.is_open()) {
       cerr << "Error opening file" << endl;
       return 1;
   }

   while (getline(inputFile, line)) {
       vector<string> parts = split(line, "   "); // Adjust delimiter if necessary
       if (parts.size() == 2) {
           a.push_back(stoi(parts[0]));
           b.push_back(stoi(parts[1]));
       }
   }

   inputFile.close();

   sort(a.begin(), a.end());
   sort(b.begin(), b.end());

   cout << "Total distance: " << calculateTotalDistance(a, b) << endl;
   cout << "Similarity score: " << calculateSimilarityScore(a, b) << endl;

   return 0;
}
