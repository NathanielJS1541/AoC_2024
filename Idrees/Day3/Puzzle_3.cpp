//#include <iostream>
//#include <fstream>
//#include <string>
//#include <cctype>
//
//// Use the standard namespace to avoid prefixing std:: before standard library components
//using namespace std;
//
//// Function to check if a character is a digit
//bool is_digit(char c) {
//    return c >= '0' && c <= '9';
//}
//
//// Function to parse a number starting at position 'start' in 's'
//// It extracts up to 'max_digits' digits and returns the number and the new position
//bool parse_number(const string& s, size_t start, int max_digits, int& number, size_t& new_pos) {
//    number = 0;
//    size_t pos = start;
//    int digits = 0;
//
//    while (pos < s.length() && is_digit(s[pos]) && digits < max_digits) {
//        number = number * 10 + (s[pos] - '0');
//        pos++;
//        digits++;
//    }
//
//    if (digits == 0) {
//        return false; // No digits found
//    }
//
//    new_pos = pos;
//    return true;
//}
//
//// Function to sum the results of valid mul(X,Y) instructions
//int sum_mul_results(const string& corrupted_memory) {
//    int total = 0;
//    size_t i = 0;
//
//    while (i < corrupted_memory.length()) {
//        // Look for 'm'
//        if (corrupted_memory[i] == 'm') {
//            // Check if the next characters are 'u', 'l', '('
//            if (i + 4 <= corrupted_memory.length() && corrupted_memory.substr(i, 4) == "mul(") {
//                size_t pos = i + 4; // Position after 'mul('
//
//                // Parse X
//                int X = 0;
//                size_t new_pos = pos;
//                if (!parse_number(corrupted_memory, pos, 3, X, new_pos)) {
//                    // Invalid X, skip this 'mul('
//                    i++;
//                    continue;
//                }
//
//                pos = new_pos;
//
//                // Check for ','
//                if (pos >= corrupted_memory.length() || corrupted_memory[pos] != ',') {
//                    // Invalid format, skip
//                    i++;
//                    continue;
//                }
//                pos++; // Move past ','
//
//                // Parse Y
//                int Y = 0;
//                if (!parse_number(corrupted_memory, pos, 3, Y, new_pos)) {
//                    // Invalid Y, skip
//                    i++;
//                    continue;
//                }
//
//                pos = new_pos;
//
//                // Check for ')'
//                if (pos >= corrupted_memory.length() || corrupted_memory[pos] != ')') {
//                    // Invalid format, skip
//                    i++;
//                    continue;
//                }
//                pos++; // Move past ')'
//
//                // Valid 'mul(X,Y)' found
//                int product = X * Y;
//                cout << "mul(" << X << "," << Y << ") = " << product << "\n";
//                total += product;
//
//                // Move 'i' to 'pos' to continue parsing after this instruction
//                i = pos;
//                continue; // Continue to next character
//            }
//        }
//        i++; // Move to next character
//    }
//
//    return total;
//}
//
//int main() {
//    // Define the input file name
//    const string filename = "input03.txt";
//
//    // Open the input file
//    ifstream infile(filename);
//    if (!infile) {
//        cerr << "Error: Unable to open file '" << filename << "'. Please ensure the file exists.\n";
//        return 1;
//    }
//
//    // Read the entire file content into a string
//    string corrupted_memory;
//    string line;
//    while (getline(infile, line)) {
//        corrupted_memory += line;
//    }
//
//    infile.close(); // Close the file
//
//    // Check if the corrupted_memory string is empty
//    if (corrupted_memory.empty()) {
//        cerr << "Error: The file '" << filename << "' is empty.\n";
//        return 1;
//    }
//
//    // Call the function to calculate the sum of valid mul instructions
//    int result = sum_mul_results(corrupted_memory);
//
//    // Print the total sum
//    cout << "Total sum of valid mul instructions: " << result << "\n";
//
//    return 0;
//}


#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

// Use the standard namespace to simplify code
using namespace std;

// Function to check if a character is a digit
bool is_digit(char c) {
   return c >= '0' && c <= '9';
}

// Function to parse a number starting at position 'start' in 's'
// It extracts up to 'max_digits' digits and returns the number and the new position
bool parse_number(const string& s, size_t start, int max_digits, int& number, size_t& new_pos) {
   number = 0;
   size_t pos = start;
   int digits = 0;

   while (pos < s.length() && is_digit(s[pos]) && digits < max_digits) {
       number = number * 10 + (s[pos] - '0');
       pos++;
       digits++;
   }

   if (digits == 0) {
       return false; // No digits found
   }

   new_pos = pos;
   return true;
}

// Function to process the corrupted memory and sum valid mul instructions
int sum_mul_results(const string& corrupted_memory) {
   int total = 0;
   size_t i = 0;
   bool mul_enabled = true; // Initially, mul instructions are enabled

   while (i < corrupted_memory.length()) {
       // Check for 'do()' instruction
       if (i + 4 <= corrupted_memory.length() && corrupted_memory.substr(i, 4) == "do()") {
           mul_enabled = true;
           cout << "Instruction: do() - Mul instructions ENABLED\n";
           i += 4; // Move past 'do()'
           continue;
       }

       // Check for "don't()" instruction
       if (i + 7 <= corrupted_memory.length() && corrupted_memory.substr(i, 7) == "don't()") {
           mul_enabled = false;
           cout << "Instruction: don't() - Mul instructions DISABLED\n";
           i += 7; // Move past "don't()"
           continue;
       }

       // Check for 'mul(' instruction
       if (i + 4 <= corrupted_memory.length() && corrupted_memory.substr(i, 4) == "mul(") {
           size_t pos = i + 4; // Position after 'mul('

           // Parse X
           int X = 0;
           size_t new_pos = pos;
           if (!parse_number(corrupted_memory, pos, 3, X, new_pos)) {
               // Invalid X, skip this 'mul('
               i++;
               continue;
           }

           pos = new_pos;

           // Check for ','
           if (pos >= corrupted_memory.length() || corrupted_memory[pos] != ',') {
               // Invalid format, skip
               i++;
               continue;
           }
           pos++; // Move past ','

           // Parse Y
           int Y = 0;
           if (!parse_number(corrupted_memory, pos, 3, Y, new_pos)) {
               // Invalid Y, skip
               i++;
               continue;
           }

           pos = new_pos;

           // Check for ')'
           if (pos >= corrupted_memory.length() || corrupted_memory[pos] != ')') {
               // Invalid format, skip
               i++;
               continue;
           }
           pos++; // Move past ')'

           // Valid 'mul(X,Y)' found
           if (mul_enabled) {
               int product = X * Y;
               cout << "mul(" << X << "," << Y << ") = " << product << "\n";
               total += product;
           }
           else {
               cout << "mul(" << X << "," << Y << ") - DISABLED\n";
           }

           // Move 'i' to 'pos' to continue parsing after this instruction
           i = pos;
           continue; // Continue to next character
       }

       // Check for other potential instructions (e.g., "undo()") if needed
       // For now, we ignore them as per the problem statement

       // Move to the next character
       i++;
   }

   return total;
}

int main() {
   // Define the input file name
   const string filename = "input03.txt";

   // Open the input file
   ifstream infile(filename);
   if (!infile) {
       cerr << "Error: Unable to open file '" << filename << "'. Please ensure the file exists.\n";
       return 1;
   }

   // Read the entire file content into a string
   string corrupted_memory;
   string line;
   while (getline(infile, line)) {
       corrupted_memory += line;
   }

   infile.close(); // Close the file

   // Check if the corrupted_memory string is empty
   if (corrupted_memory.empty()) {
       cerr << "Error: The file '" << filename << "' is empty.\n";
       return 1;
   }

   // Call the function to calculate the sum of valid mul instructions
   int result = sum_mul_results(corrupted_memory);

   // Print the total sum
   cout << "Total sum of enabled mul instructions: " << result << "\n";

   return 0;
}
