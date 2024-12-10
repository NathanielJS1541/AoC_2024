#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <sstream>

using namespace std;

int main()
{

    // Read in the rules as  an unordered map
    ifstream inputFile("input.txt");

    string line;

    // Unordered map of instructions first page
    unordered_map<int, unique_ptr<vector<int>>> instructions;

    bool readinstr = true;
    bool bad = false;

    int sum = 0;
    while (getline(inputFile, line))
    {
        if (line == "")
        {
            readinstr = false;
        }
        else
        {

            if (readinstr)
            {

                string key = line.substr(0, 2);
                string value = line.substr(3, 4);

                // get the vector already stored at this key
                if (instructions.find(strtoul(key.c_str(), nullptr, 10)) == instructions.end())
                {
                    instructions[strtoul(key.c_str(), nullptr, 10)] = make_unique<vector<int>>(1, strtoul(value.c_str(), nullptr, 10));
                }
                else
                {
                    instructions.at(strtoul(key.c_str(), nullptr, 10)).get()->push_back(strtoul(value.c_str(), nullptr, 10));
                }
            }
            else
            {
                // Check the each output one by one against every rule
                // Sum the middle number if correct
                // Splitting the str string by delimiter
                vector<int> output;
                stringstream streamline(line);
                string t;

                cout << "Output: " << endl;

                while (getline(streamline, t, ','))
                {
                    output.push_back(stoi(t));
                    cout << t << ",";
                }

                cout << "\n";

                vector<int> *instructionSet;
                bad = false;

                // go through each number in the output
                for (int i = 1; i < output.size(); i++)
                {
                    // get the vector already stored at this key
                    if (instructions.find(output[i]) != instructions.end())
                    {
                        instructionSet = instructions.at(output[i]).get();

                        // get the relevant instruction i.e. which numbers cannot be before output[i]
                        for (int k = 0; k < instructionSet->size(); k++)
                        {

                            // check if each instruction equals any number before this one in the output
                            for (int j = i - 1; j >= 0; j--)
                            {
                                if (j == 0 && k == 5)
                                {
                                    cout << output[j] << " " << instructionSet->at(k) << endl;
                                }
                                // if any value before output[i] is in the instruction set then break as this is a bad case
                                if (output[j] == instructionSet->at(k))
                                {
                                    // end the search this is a bad entry
                                    cout << "The above was a bad entry\n"
                                         << endl;
                                    bad = true;
                                    j = 0;
                                    k = instructionSet->size();
                                    i = output.size();
                                    break;
                                }
                            }
                        }
                    }

                    if (i == output.size() - 1 && !bad)
                    {
                        sum += output[output.size() / 2];
                        cout << "The middle number is: " << output[output.size() / 2] << endl;
                        cout << "The above was a success\n"
                             << endl;
                    }
                }
            }
        }
    }
    cout << "The sum of the middle numbers is: " << sum << endl;
}