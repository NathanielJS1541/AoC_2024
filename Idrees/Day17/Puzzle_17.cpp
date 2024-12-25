////#include <iostream>
////#include <vector>
////#include <string>
////#include <climits>
////
////using namespace std;
////
////void run(int regA, const vector<int>& Prog, vector<int>& Output) {
////    int RegA = regA, RegB = 0, RegC = 0, IP = 0;
////
////    while (IP < Prog.size()) {
////        int instr = Prog[IP], operand = Prog[IP + 1];
////        int operand_value = (operand < 4) ? operand : (operand == 4 ? RegA : (operand == 5 ? RegB : RegC));
////
////        switch (instr) {
////        case 0:
////            RegA >>= operand_value;
////            break;
////        case 1:
////            RegB ^= operand;
////            break;
////        case 2:
////            RegB = operand_value % 8;
////            break;
////        case 3:
////            if (RegA != 0) IP = operand - 2;
////            break;
////        case 4:
////            RegB ^= RegC;
////            break;
////        case 5:
////            Output.push_back(operand_value % 8);
////            RegB = operand_value % 8;
////            break;
////        case 6:
////            RegB = RegA >> operand_value;
////            break;
////        case 7:
////            RegC = RegA >> operand_value;
////            break;
////        default:
////            cout << "Invalid instruction!" << endl;
////            return;
////        }
////
////        IP += 2;
////    }
////}
////
////string part1(int regA, const vector<int>& Prog) {
////    vector<int> Output;
////    run(regA, Prog, Output);
////    string result;
////    for (size_t i = 0; i < Output.size(); i++) {
////        result += to_string(Output[i]);
////        if (i < Output.size() - 1) result += ",";
////    }
////    return result;
////}
////
////long long part2(const vector<int>& prog, long long curr, int i) {
////    if (i == -1) return curr;
////    for (int j = 0; j < 8; j++) {
////        long long RegA = (8 * curr) + j, RegB = 0;
////
////        RegB = ((RegA % 8) ^ 1);
////        long long temp = (RegA >> RegB);
////        RegB ^= (5 ^ temp);
////
////        if (RegB % 8 == prog[i]) {
////            long long result = part2(prog, RegA, i - 1);
////            if (result) return result;
////        }
////    }
////    return 0;
////}
////
////int main() {
////    int regA = 30118712;
////    vector<int> Prog_Vector = { 2,4,1,3,7,5,4,2,0,3,1,5,5,5,3,0 };
////
////    std::cout << "Part1: " << part1(regA, Prog_Vector) << endl;
////    std::cout << "Part2: " << part2(Prog_Vector, 0, Prog_Vector.size() - 1) << endl;
////
////    return 0;
////}
//
//#include <iostream>
//#include <vector>
//#include <string>
//#include <climits>
//#include <chrono>
//
//using namespace std;
//
//// Function to run the program and collect outputs
//void run(int regA, const vector<int>& Prog, vector<int>& Output) {
//    int RegA = regA, RegB = 0, RegC = 0, IP = 0;
//
//    while (IP < Prog.size()) {
//        if (IP + 1 >= Prog.size()) break; // Prevent out-of-bounds access
//        int instr = Prog[IP], operand = Prog[IP + 1];
//        int operand_value = (operand < 4) ? operand : (operand == 4 ? RegA : (operand == 5 ? RegB : RegC));
//
//        switch (instr) {
//        case 0:
//            if ((1 << operand_value) == 0) { // Prevent division by zero
//                cout << "Division by zero encountered!" << endl;
//                return;
//            }
//            RegA >>= operand_value;
//            break;
//        case 1:
//            RegB ^= operand;
//            break;
//        case 2:
//            RegB = operand_value % 8;
//            break;
//        case 3:
//            if (RegA != 0) {
//                // Jump to the operand position (operand is zero-based index)
//                if (operand < 0 || operand >= Prog.size()) {
//                    cout << "Invalid jump address!" << endl;
//                    return;
//                }
//                IP = operand;
//                continue; // Skip the IP increment at the end
//            }
//            break;
//        case 4:
//            RegB ^= RegC;
//            break;
//        case 5:
//            Output.push_back(operand_value % 8);
//            RegB = operand_value % 8;
//            break;
//        case 6:
//            RegB = RegA >> operand_value;
//            break;
//        case 7:
//            RegC = RegA >> operand_value;
//            break;
//        default:
//            cout << "Invalid instruction!" << endl;
//            return;
//        }
//
//        IP += 2;
//    }
//}
//
//// Function to convert output vector to comma-separated string
//string outputToString(const vector<int>& Output) {
//    string result;
//    for (size_t i = 0; i < Output.size(); i++) {
//        result += to_string(Output[i]);
//        if (i < Output.size() - 1) result += ",";
//    }
//    return result;
//}
//
//// Function for Part 1
//string part1(int regA, const vector<int>& Prog) {
//    vector<int> Output;
//    run(regA, Prog, Output);
//    return outputToString(Output);
//}
//
//// Function for Part 2
//long long part2(const vector<int>& prog) {
//    long long regA = 1;
//    const long long MAX_REGA = 1000000000; // Set a reasonable upper limit to prevent infinite loops
//    size_t prog_size = prog.size();
//
//    // Precompute the expected output as the program itself
//    vector<int> expectedOutput = prog;
//
//    // Start timing to monitor performance
//    auto start_time = chrono::high_resolution_clock::now();
//
//    while (regA < MAX_REGA) {
//        vector<int> Output;
//        run(regA, prog, Output);
//
//        // Check if the output matches the program
//        if (Output.size() == prog_size) {
//            bool match = true;
//            for (size_t i = 0; i < prog_size; i++) {
//                if (Output[i] != prog[i]) {
//                    match = false;
//                    break;
//                }
//            }
//            if (match) {
//                // Optionally, print the time taken
//                auto end_time = chrono::high_resolution_clock::now();
//                chrono::duration<double> elapsed = end_time - start_time;
//                cout << "Time taken: " << elapsed.count() << " seconds\n";
//                return regA;
//            }
//        }
//
//        // Increment regA
//        regA++;
//
//        // Optional: Print progress every million iterations
//        if (regA % 1000000 == 0) {
//            auto current_time = chrono::high_resolution_clock::now();
//            chrono::duration<double> elapsed = current_time - start_time;
//            cout << "Checked regA = " << regA << " | Time elapsed: " << elapsed.count() << " seconds\n";
//        }
//    }
//
//    // If no solution is found within the limit
//    cout << "No valid regA found within the limit of " << MAX_REGA << endl;
//    return -1;
//}
//
//int main() {
//    // Example input based on your problem description
//    int regA_initial = 30118712;
//    vector<int> Prog_Vector = { 2,4,1,3,7,5,4,2,0,3,1,5,5,5,3,0 };
//
//    // Part 1
//    string output_part1 = part1(regA_initial, Prog_Vector);
//    cout << "Part1: " << output_part1 << endl;
//
//    // Part 2
//    // Note: This might take a considerable amount of time depending on the program and required regA
//    cout << "Starting Part2..." << endl;
//    long long regA_part2 = part2(Prog_Vector);
//    if (regA_part2 != -1) {
//        cout << "Part2: The smallest initial value for register A is " << regA_part2 << endl;
//    }
//    else {
//        cout << "Part2: No valid initial value for register A found within the limit." << endl;
//    }
//
//    return 0;
//}




#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct algoState
{
	long long posPosition;
	int workingDigit;

	algoState(long long pos, int curWorkingDigit)
	{
		posPosition = pos;
		workingDigit = curWorkingDigit;
	}
};

// Methods
void readNumbers(vector<int>& list, string nums);
void readRegisterNumber(long long& registerNum, string registerString);
void computeOperation(int opCode, int operand, long long& registerA, long long& registerB, long long& registerC,
	int& pointer, string& output);
long long findComboOperand(int operand, long long registerA, long long registerB, long long registerC);
bool numberListsEqual(string curOutput, const vector<int>& instructionList);
string algorithm(vector<int>& instructionList, long long registerA);
bool checkDigits(string curOutput, const vector<int>& instructionList, int curDigit);

int main()
{

	// Import the data
	ifstream fileRead("input17.txt", ifstream::in);

	// Parse the data
	string input;

	//Important variables in problem
	long long registerA;
	long long registerB;
	long long registerC;

	getline(fileRead, input);
	readRegisterNumber(registerA, input);
	getline(fileRead, input);
	readRegisterNumber(registerB, input);
	getline(fileRead, input);
	readRegisterNumber(registerC, input);

	string output;

	getline(fileRead, input);

	while (!input.size()) //While there are spaces, get next line
	{
		getline(fileRead, input);

	}

	// Make a vector for the numbers in the row
	vector<int> instructionList;

	//Get the numbers
	readNumbers(instructionList, input);

	long long firstA = 0;

	/*for (int i = 0; i < 101; i++)
	{
		long long posA = curA + i;
		algorithm(instructionList, posA);
	}*/

	long long firstWorkingDigit = 1; // First looking for last digit

	queue<algoState> posStates;
	posStates.push(algoState(firstA, firstWorkingDigit));

	while (!posStates.empty())
	{
		algoState curState = posStates.front();
		posStates.pop();

		long long curA = curState.posPosition;
		int curWorkingDigit = curState.workingDigit;

		for (int i = 0; i < 8; i++)
		{
			long long posA = curA + i;
			output = algorithm(instructionList, posA);

			//If the amount of digits are the same from the end,
			//break and move on
			if (checkDigits(output, instructionList, curWorkingDigit))
			{

				if (curWorkingDigit == 16 && numberListsEqual(output, instructionList))
				{
					cout << "Solution A: " << posA << endl;
					cout << output << endl << endl;
				}
				else if (curWorkingDigit < 16)
				{
					long long nextA = posA;
					int nextWorkingDigit = curWorkingDigit + 1;

					//Find the next curA to start with
					if (curWorkingDigit == 1)
					{
						nextA = pow(8, curWorkingDigit) + (8 * posA) - 8;
						algoState newState(nextA, nextWorkingDigit);
						posStates.push(newState);
					}
					else
					{
						long long basisNum = posA - pow(8, curWorkingDigit - 1);
						nextA = pow(8, curWorkingDigit) + (basisNum * 8);
						algoState newState(nextA, nextWorkingDigit);
						posStates.push(newState);
					}
				}

			}
		}
	}

	return 0;
}

bool checkDigits(string curOutput, const vector<int>& instructionList, int curDigit)
{
	vector<int> outputList;
	readNumbers(outputList, curOutput);

	int curOutputIndex = outputList.size() - 1;

	for (int i = instructionList.size() - 1; i > instructionList.size() - 1 - curDigit; i--)
	{
		if (instructionList[i] != outputList[curOutputIndex])
		{
			return false;
		}

		curOutputIndex--;
	}

	return true;
}

//Checks to see if output and program are the same
bool numberListsEqual(string curOutput, const vector<int>& instructionList)
{
	vector<int> outputList;
	readNumbers(outputList, curOutput);

	if (instructionList.size() != outputList.size())
	{
		return false;
	}

	for (int i = 0; i < instructionList.size(); i++)
	{
		if (outputList[i] != instructionList[i])
		{
			return false;
		}
	}

	return true;
}

string algorithm(vector<int>& instructionList, long long registerA)
{

	long long i = registerA;

	long long registerB = 0;
	long long registerC = 0;

	//Starting pointer for instructions
	int instructPointer = 0;

	//String output string
	string output;

	/*int opCompleted = 1;*/

	//Keep going while instruction pointer is still in bounds 
	while (instructPointer + 1 < instructionList.size())
	{
		int opCode = instructionList[instructPointer];
		int operand = instructionList[instructPointer + 1];

		//Compute operation
		computeOperation(opCode, operand, registerA, registerB, registerC, instructPointer, output);

		if (opCode != 3)
		{
			instructPointer += 2;
		}

		/*cout << "Operation Completed" << opCompleted << endl;
		opCompleted++;*/
	}

	//cout << "A: " << i << " " << "Output is below: " << endl;
	//cout << output.substr(0, output.find_last_of(',')) << endl; //Gets rid of the extra ',' at the end

	return output;
}

//In charge of doing the main beefy part of the problem (operations)
void computeOperation(int opCode, int operand, long long& registerA, long long& registerB, long long& registerC,
	int& pointer, string& output)
{
	if (opCode == 0)
	{
		long long numerator = registerA;
		long long denominator = pow(2, findComboOperand(operand, registerA, registerB, registerC));
		long long divResult = numerator / denominator;
		registerA = divResult;
	}
	else if (opCode == 1)
	{
		registerB = registerB ^ operand;
	}
	else if (opCode == 2)
	{
		long long comboOperand = findComboOperand(operand, registerA, registerB, registerC);
		registerB = comboOperand % 8;
	}
	else if (opCode == 3)
	{
		if (registerA != 0)
		{
			pointer = operand;
		}
		else
		{
			pointer += 2;
		}
	}
	else if (opCode == 4)
	{
		registerB = registerB ^ registerC;
	}
	else if (opCode == 5)
	{
		long long modResult = findComboOperand(operand, registerA, registerB, registerC) % 8;
		string modString = to_string(modResult);

		//If more than one digit, separate by comma
		for (char c : modString)
		{
			output += c;
			output += ',';
		}
	}
	else if (opCode == 6)
	{
		long long numerator = registerA;
		long long denominator = pow(2, findComboOperand(operand, registerA, registerB, registerC));
		long long divResult = numerator / denominator;
		registerB = divResult;
	}
	else if (opCode == 7)
	{
		long long numerator = registerA;
		long long denominator = pow(2, findComboOperand(operand, registerA, registerB, registerC));
		long long divResult = numerator / denominator;
		registerC = divResult;
	}
}

//Returns the "combo operand"
long long findComboOperand(int operand, long long registerA, long long registerB, long long registerC)
{
	if (operand >= 0 && operand <= 3)
	{
		return operand;
	}
	else if (operand == 4)
	{
		return registerA;
	}
	else if (operand == 5)
	{
		return registerB;
	}
	else if (operand == 6)
	{
		return registerC;
	}
	else if (operand == 7)
	{
		cout << "Invalid" << endl;
		return -1;
	}

	return -1;
}

void readRegisterNumber(long long& registerNum, string registerString)
{
	string numString = "";
	for (int i = 0; i < registerString.size(); i++)
	{
		char c = registerString[i];
		if (48 <= c && c <= 57)
		{
			numString += registerString[i];
		}
		else if (numString != "" && c == ' ')
		{
			registerNum = stoll(numString);
			return;
		}
	}

	if (numString != "")
	{
		registerNum = stoll(numString);
		return;
	}
}

void readNumbers(vector<int>& list, string nums)
{
	string numString = "";
	for (int i = 0; i < nums.size(); i++)
	{
		char c = nums[i];
		if (48 <= c && c <= 57)
		{
			numString += nums[i];
		}
		else if (numString != "" && c == ',')
		{
			list.push_back(stoi(numString));
			numString = "";
		}
	}

	if (numString != "")
	{
		list.push_back(stoi(numString));
	}
}