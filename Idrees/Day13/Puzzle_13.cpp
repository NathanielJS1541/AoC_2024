//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//#include <vector>
//#include <algorithm>
//#include <climits>
//
//using namespace std;
//
//// Structure to hold button movement and cost
//struct Button {
//    int X;
//    int Y;
//    int cost;
//};
//
//// Structure to hold prize location
//struct Prize {
//    int X;
//    int Y;
//};
//
//// Structure to hold a claw machine's configuration
//struct ClawMachine {
//    Button A;
//    Button B;
//    Prize prize;
//};
//
//// Function to trim whitespace from start and end of a string
//string trim(const string& str) {
//    size_t first = str.find_first_not_of(" \t\r\n");
//    if (first == string::npos)
//        return "";
//    size_t last = str.find_last_not_of(" \t\r\n");
//    return str.substr(first, (last - first + 1));
//}
//
//// Function to parse a line like "Button A: X+94, Y+34" or "Prize: X=8400, Y=5400"
//bool parseLine(const string& line, Button& button, Prize& prize, bool isButtonA) {
//    stringstream ss(line);
//    string label, coord, value;
//
//    if (line.find("Button A") != string::npos || line.find("Button B") != string::npos) {
//        // Parsing button line
//        getline(ss, label, ':'); // "Button A" or "Button B"
//        getline(ss, coord, ','); // " X+94"
//        // Parse X
//        size_t plusPos = coord.find('+');
//        if (plusPos == string::npos) return false;
//        button.X = stoi(coord.substr(plusPos + 1));
//
//        getline(ss, coord); // " Y+34"
//        plusPos = coord.find('+');
//        if (plusPos == string::npos) return false;
//        button.Y = stoi(coord.substr(plusPos + 1));
//
//        button.cost = isButtonA ? 3 : 1;
//        return true;
//    }
//    else if (line.find("Prize") != string::npos) {
//        // Parsing prize line
//        getline(ss, label, ':'); // "Prize"
//        getline(ss, coord, ','); // " X=8400"
//        size_t eqPos = coord.find('=');
//        if (eqPos == string::npos) return false;
//        prize.X = stoi(coord.substr(eqPos + 1));
//
//        getline(ss, coord); // " Y=5400"
//        eqPos = coord.find('=');
//        if (eqPos == string::npos) return false;
//        prize.Y = stoi(coord.substr(eqPos + 1));
//
//        return true;
//    }
//
//    return false;
//}
//
//int main() {
//    ifstream file("input13.txt");
//    if (!file) {
//        cerr << "Error: Could not open input13.txt" << endl;
//        return 1;
//    }
//
//    string line;
//    vector<ClawMachine> machines;
//    ClawMachine currentMachine;
//    bool readingMachine = false;
//
//    while (getline(file, line)) {
//        line = trim(line);
//        if (line.empty()) continue; // Skip empty lines
//
//        if (line.find("Button A:") != string::npos) {
//            // Start of a new machine
//            currentMachine = ClawMachine(); // Reset current machine
//            // Parse Button A
//            if (!parseLine(line, currentMachine.A, currentMachine.prize, true)) {
//                cerr << "Error parsing Button A line: " << line << endl;
//                return 1;
//            }
//            readingMachine = true;
//        }
//        else if (line.find("Button B:") != string::npos && readingMachine) {
//            // Parse Button B
//            if (!parseLine(line, currentMachine.B, currentMachine.prize, false)) {
//                cerr << "Error parsing Button B line: " << line << endl;
//                return 1;
//            }
//        }
//        else if (line.find("Prize:") != string::npos && readingMachine) {
//            // Parse Prize
//            if (!parseLine(line, currentMachine.A, currentMachine.prize, false)) {
//                cerr << "Error parsing Prize line: " << line << endl;
//                return 1;
//            }
//            // Add the completed machine to the list
//            machines.push_back(currentMachine);
//            readingMachine = false;
//        }
//        // Else: lines that are not part of the machine configuration are ignored
//    }
//
//    file.close();
//
//    // Now process each machine to find the minimal cost
//    long long totalCost = 0;
//    int machinesWithPrizes = 0;
//
//    for (size_t i = 0; i < machines.size(); ++i) {
//        ClawMachine& machine = machines[i];
//        int Xt = machine.prize.X;
//        int Yt = machine.prize.Y;
//        int Xa = machine.A.X;
//        int Ya = machine.A.Y;
//        int Xb = machine.B.X;
//        int Yb = machine.B.Y;
//        int costA = machine.A.cost;
//        int costB = machine.B.cost;
//
//        int minCost = INT32_MAX;
//
//        // Iterate over possible presses of Button A
//        for (int a = 0; a <= 100; ++a) {
//            // Calculate remaining X after pressing A 'a' times
//            int remainingX = Xt - (Xa * a);
//            if (Xb == 0) {
//                if (remainingX != 0) continue;
//            }
//            else {
//                if (remainingX < 0) continue;
//                if (remainingX % Xb != 0) continue;
//            }
//
//            int b = (Xb == 0) ? 0 : remainingX / Xb;
//
//            if (b < 0 || b > 100) continue;
//
//            // Now check if Y matches
//            int totalY = (Ya * a) + (Yb * b);
//            if (totalY == Yt) {
//                // Valid solution found
//                int currentCost = (costA * a) + (costB * b);
//                if (currentCost < minCost) {
//                    minCost = currentCost;
//                }
//            }
//        }
//
//        if (minCost != INT32_MAX) {
//            totalCost += minCost;
//            machinesWithPrizes++;
//            // Debug output (optional)
//            // cout << "Machine " << i+1 << ": Min Cost = " << minCost << " tokens (A presses: " << a << ", B presses: " << b << ")" << endl;
//        }
//        else {
//            // No solution for this machine
//            // Debug output (optional)
//            // cout << "Machine " << i+1 << ": No solution found." << endl;
//        }
//    }
//
//    cout << "Total prizes won: " << machinesWithPrizes << endl;
//    cout << "Minimum total tokens required: " << totalCost << endl;
//
//    return 0;
//}


#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

// Function to extract all integers from a string
vector<long long> extract_numbers(const string& line) {
    regex rgx("[-+]?[0-9]+");
    sregex_iterator it(line.begin(), line.end(), rgx);
    sregex_iterator end;
    vector<long long> numbers;
    while (it != end) {
        numbers.push_back(stoll((*it).str()));
        ++it;
    }
    return numbers;
}

int main() {
    ifstream infile("input13.txt");
    if (!infile.is_open()) {
        cerr << "Error: Could not open input13.txt" << endl;
        return 1;
    }

    string line;
    long long total_tokens = 0;
    const long long OFFSET = 10000000000000LL; // 10^13

    // Regular expressions to identify lines
    regex buttonA_regex("^Button A:");
    regex buttonB_regex("^Button B:");
    regex prize_regex("^Prize:");

    while (getline(infile, line)) {
        // Skip lines that don't start with "Button A:"
        if (!regex_search(line, buttonA_regex)) {
            continue;
        }

        // Process Button A
        vector<long long> numbersA = extract_numbers(line);
        if (numbersA.size() < 2) {
            cerr << "Error: Invalid format for Button A line: " << line << endl;
            return 1;
        }
        long long A_X = numbersA[0];
        long long A_Y = numbersA[1];

        // Read Button B
        if (!getline(infile, line)) {
            cerr << "Error: Unexpected end of file when reading Button B." << endl;
            return 1;
        }
        if (!regex_search(line, buttonB_regex)) {
            cerr << "Error: Expected 'Button B' line, but got: " << line << endl;
            return 1;
        }
        vector<long long> numbersB = extract_numbers(line);
        if (numbersB.size() < 2) {
            cerr << "Error: Invalid format for Button B line: " << line << endl;
            return 1;
        }
        long long B_X = numbersB[0];
        long long B_Y = numbersB[1];

        // Read Prize
        if (!getline(infile, line)) {
            cerr << "Error: Unexpected end of file when reading Prize." << endl;
            return 1;
        }
        if (!regex_search(line, prize_regex)) {
            cerr << "Error: Expected 'Prize' line, but got: " << line << endl;
            return 1;
        }
        vector<long long> numbersP = extract_numbers(line);
        if (numbersP.size() < 2) {
            cerr << "Error: Invalid format for Prize line: " << line << endl;
            return 1;
        }
        long long P_X = numbersP[0] + OFFSET;
        long long P_Y = numbersP[1] + OFFSET;

        // Solve the system of equations:
        // A_X * a + B_X * b = P_X
        // A_Y * a + B_Y * b = P_Y
        long long D = A_X * B_Y - A_Y * B_X;

        if (D == 0) {
            // No unique solution exists
            continue;
        }

        long long a_num = P_X * B_Y - P_Y * B_X;
        long long b_num = A_X * P_Y - A_Y * P_X;

        // Handle the sign of D
        if (D < 0) {
            D = -D;
            a_num = -a_num;
            b_num = -b_num;
        }

        // Check if D divides both a_num and b_num
        if (a_num % D != 0 || b_num % D != 0) {
            // No integer solution exists
            continue;
        }

        long long a = a_num / D;
        long long b = b_num / D;

        // Check if a and b are non-negative
        if (a < 0 || b < 0) {
            continue;
        }

        // Calculate the total tokens for this machine
        long long tokens = 3 * a + b;
        total_tokens += tokens;
    }

    infile.close();

    cout << "Minimum total tokens required to win all possible prizes: " << total_tokens << endl;

    return 0;
}
