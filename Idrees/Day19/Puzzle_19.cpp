////#include <iostream>
////#include <fstream>
////#include <vector>
////#include <string>
////#include <unordered_set>
////#include <sstream>
////
////using namespace std;
////
////// Function to split a string by a delimiter and return a vector of trimmed tokens
////vector<string> split(const string& s, char delimiter) {
////    vector<string> tokens;
////    string token;
////    stringstream ss(s);
////    while (getline(ss, token, delimiter)) {
////        // Trim whitespace from token
////        size_t start = token.find_first_not_of(" \t\r\n");
////        size_t end = token.find_last_not_of(" \t\r\n");
////        if (start != string::npos && end != string::npos)
////            tokens.push_back(token.substr(start, end - start + 1));
////        else if (start != string::npos)
////            tokens.push_back(token.substr(start));
////        // else skip empty tokens
////    }
////    return tokens;
////}
////
////int main() {
////    // Open the input file
////    ifstream infile("input19.txt");
////    if (!infile.is_open()) {
////        cerr << "Error: Could not open input19.txt" << endl;
////        return 1;
////    }
////
////    string line;
////    // Read the first line: available towel patterns
////    if (!getline(infile, line)) {
////        cerr << "Error: Input file is empty." << endl;
////        return 1;
////    }
////
////    // Split the first line by commas to get patterns
////    vector<string> patterns = split(line, ',');
////
////    // Insert patterns into an unordered_set for O(1) lookups
////    unordered_set<string> patternSet;
////    for (const string& p : patterns) {
////        if (!p.empty())
////            patternSet.insert(p);
////    }
////
////    // Read until a blank line is found
////    while (getline(infile, line)) {
////        if (line.empty())
////            break;
////    }
////
////    // Now, read the desired designs
////    vector<string> designs;
////    while (getline(infile, line)) {
////        if (!line.empty())
////            designs.push_back(line);
////    }
////
////    infile.close();
////
////    // Function to check if a design can be segmented into patterns
////    auto canSegment = [&](const string& design) -> bool {
////        int n = design.size();
////        vector<bool> dp(n + 1, false);
////        dp[0] = true; // Empty string can be segmented
////
////        for (int i = 1; i <= n; ++i) {
////            for (int j = 0; j < i; ++j) {
////                if (dp[j]) {
////                    string substr = design.substr(j, i - j);
////                    if (patternSet.find(substr) != patternSet.end()) {
////                        dp[i] = true;
////                        break;
////                    }
////                }
////            }
////        }
////
////        return dp[n];
////        };
////
////    // Count the number of possible designs
////    int possibleCount = 0;
////    for (const string& design : designs) {
////        if (canSegment(design)) {
////            possibleCount++;
////        }
////    }
////
////    // Output the result
////    cout << possibleCount << endl;
////
////    return 0;
////}
//
//
//
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <string>
//#include <unordered_set>
//#include <sstream>
//#include <algorithm>
//
//using namespace std;
//
//// Function to split a string by a delimiter and return a vector of trimmed tokens
//vector<string> split(const string& s, char delimiter) {
//    vector<string> tokens;
//    string token;
//    stringstream ss(s);
//    while (getline(ss, token, delimiter)) {
//        // Trim whitespace from token
//        size_t start = token.find_first_not_of(" \t\r\n");
//        size_t end = token.find_last_not_of(" \t\r\n");
//        if (start != string::npos && end != string::npos)
//            tokens.push_back(token.substr(start, end - start + 1));
//        else if (start != string::npos)
//            tokens.push_back(token.substr(start));
//        // else skip empty tokens
//    }
//    return tokens;
//}
//
//int main() {
//    // Open the input file
//    ifstream infile("input19.txt");
//    if (!infile.is_open()) {
//        cerr << "Error: Could not open input19.txt" << endl;
//        return 1;
//    }
//
//    string line;
//    // Read the first line: available towel patterns
//    if (!getline(infile, line)) {
//        cerr << "Error: Input file is empty." << endl;
//        return 1;
//    }
//
//    // Split the first line by commas to get patterns
//    vector<string> patterns = split(line, ',');
//
//    // Insert patterns into an unordered_set for O(1) lookups
//    unordered_set<string> patternSet;
//    for (const string& p : patterns) {
//        if (!p.empty())
//            patternSet.insert(p);
//    }
//
//    // Read until a blank line is found
//    while (getline(infile, line)) {
//        if (line.empty())
//            break;
//    }
//
//    // Now, read the desired designs
//    vector<string> designs;
//    while (getline(infile, line)) {
//        if (!line.empty())
//            designs.push_back(line);
//    }
//
//    infile.close();
//
//    // Function to check the number of ways a design can be segmented into patterns
//    auto countWays = [&](const string& design) -> long long {
//        int n = design.size();
//        // Use a vector of long long to store the number of ways
//        vector<long long> dp(n + 1, 0);
//        dp[0] = 1; // Empty string has one way
//
//        for (int i = 1; i <= n; ++i) {
//            for (int j = 0; j < i; ++j) {
//                // If dp[j] is 0, skip
//                if (dp[j] == 0)
//                    continue;
//                // Extract substring from j to i
//                string substr = design.substr(j, i - j);
//                if (patternSet.find(substr) != patternSet.end()) {
//                    dp[i] += dp[j];
//                }
//            }
//        }
//
//        return dp[n];
//        };
//
//    // Calculate the total number of ways across all designs
//    long long totalWays = 0;
//    for (const string& design : designs) {
//        long long ways = countWays(design);
//        // Uncomment the following line to see the number of ways for each design
//        // cout << "Design: " << design << " | Ways: " << ways << endl;
//        totalWays += ways;
//    }
//
//    // Output the result
//    cout << totalWays << endl;
//
//    return 0;
//}
