//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <sstream>
//#include <string>
//
//using namespace std;
//
//long long eval_left_to_right(const vector<long long>& numbers, const vector<char>& operators) {
//    long long result = numbers[0];
//    for (size_t i = 0; i < operators.size(); ++i) {
//        if (operators[i] == '+') {
//            result += numbers[i + 1];
//        }
//        else if (operators[i] == '*') {
//            result *= numbers[i + 1];
//        }
//    }
//    return result;
//}
//
//bool check_combinations(const vector<long long>& numbers, long long test_value) {
//    int num_ops = numbers.size() - 1;
//    int max_combinations = 1 << num_ops;  // 2^num_ops possible combinations
//    vector<char> ops(num_ops);
//
//    for (int i = 0; i < max_combinations; ++i) {
//        // Set up the operator array for this combination
//        for (int j = 0; j < num_ops; ++j) {
//            ops[j] = (i & (1 << j)) ? '*' : '+';  // Binary mapping to operators
//        }
//        // Evaluate this particular combination
//        long long result = eval_left_to_right(numbers, ops);
//        if (result == test_value) {
//            return true;  // Found a valid combination
//        }
//    }
//    return false;  // No valid combinations found
//}
//
//long long process_line(const string& line) {
//    stringstream ss(line);
//    long long test_value;
//    ss >> test_value;
//    vector<long long> numbers;
//    string temp;
//    getline(ss, temp, ':');  // Ignore up to the colon.
//    getline(ss, temp);        // Get the numbers part.
//    stringstream numStream(temp);
//    long long num;
//    while (numStream >> num) {
//        numbers.push_back(num);
//    }
//
//    if (numbers.empty()) return 0;
//
//    return check_combinations(numbers, test_value) ? test_value : 0;
//}
//
//int main() {
//    ifstream file("input07.txt");
//    string line;
//    long long sum = 0;
//
//    if (!file.is_open()) {
//        cerr << "Error opening file" << endl;
//        return 1;
//    }
//
//    while (getline(file, line)) {
//        if (!line.empty()) {
//            sum += process_line(line);
//        }
//    }
//
//    file.close();
//
//    cout << "Total Sum: " << sum << endl;
//    return 0;
//}


#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

// Precompute powers of 10 up to 18 digits to handle concatenation efficiently
const long long pow10_arr[19] = {
   1LL,
   10LL,
   100LL,
   1000LL,
   10000LL,
   100000LL,
   1000000LL,
   10000000LL,
   100000000LL,
   1000000000LL,
   10000000000LL,
   100000000000LL,
   1000000000000LL,
   10000000000000LL,
   100000000000000LL,
   1000000000000000LL,
   10000000000000000LL,
   100000000000000000LL,
   1000000000000000000LL
};

// Inline function to apply an operator efficiently
inline long long apply_operator(long long a, long long b, int op) {
   if (op == 0) { // '+'
       return a + b;
   }
   else if (op == 1) { // '*'
       return a * b;
   }
   else { // '||' concatenation
       if (b == 0) {
           return a * 10 + b;
       }
       int digits = 0;
       long long temp = b;
       while (temp > 0) {
           digits++;
           temp /= 10;
       }
       return a * pow10_arr[digits] + b;
   }
}

// Function to check if a combination of operators can produce the test value
bool can_match(const vector<long long>& numbers, long long target, int num_operator_types) {
   int num_ops = numbers.size() - 1;
   if (num_ops == 0) {
       return (numbers[0] == target);
   }

   // Calculate the total number of combinations (num_operator_types ^ num_ops)
   long long max_combinations = 1;
   for (int i = 0; i < num_ops; ++i) {
       max_combinations *= num_operator_types;
   }

   // Iterate through all possible operator combinations
   for (long long comb = 0; comb < max_combinations; ++comb) {
       long long result = numbers[0];
       long long current = comb;
       bool matched = false;

       for (int j = 0; j < num_ops; ++j) {
           int op = current % num_operator_types;
           current /= num_operator_types;
           result = apply_operator(result, numbers[j + 1], op);
           // Early termination is not straightforward due to the nature of operations
       }

       if (result == target) {
           return true; // Found a valid combination
       }
   }
   return false; // No valid combinations found
}

int main() {
   ifstream file("input07.txt");
   if (!file.is_open()) {
       cerr << "Error opening file" << endl;
       return 1;
   }

   string line;
   long long sum_part1 = 0;
   long long sum_part2 = 0;

   while (getline(file, line)) {
       if (line.empty()) continue;

       // Find the position of the colon
       size_t colon_pos = line.find(':');
       if (colon_pos == string::npos) {
           // Invalid line format
           continue;
       }

       // Extract and parse the test value
       string test_value_str = line.substr(0, colon_pos);
       long long test_value;
       try {
           test_value = stoll(test_value_str);
       }
       catch (...) {
           // Handle invalid number format
           continue;
       }

       // Extract and parse the numbers after the colon
       vector<long long> numbers;
       string numbers_str = line.substr(colon_pos + 1);
       stringstream numStream(numbers_str);
       long long num;
       while (numStream >> num) {
           numbers.push_back(num);
       }

       if (numbers.empty()) {
           // No numbers to process
           continue;
       }

       // Part 1: Check using only '+' and '*'
       bool part1_match = can_match(numbers, test_value, 2);
       if (part1_match) {
           sum_part1 += test_value;
           sum_part2 += test_value; // Part 2 includes Part 1 matches
           continue; // No need to check Part 2 further
       }

       // Part 2: Check using '+', '*', and '||'
       bool part2_match = can_match(numbers, test_value, 3);
       if (part2_match) {
           sum_part2 += test_value;
       }
   }

   file.close();

   cout << "Total Sum for Part 1: " << sum_part1 << endl;
   cout << "Total Sum for Part 2: " << sum_part2 << endl;

   return 0;
}
