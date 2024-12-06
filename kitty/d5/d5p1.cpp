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

    int sum = 0;
    while (getline(inputFile, line))
    {
        if (line == "")
        {
            readinstr = false;
        }

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
                instructions.at(strtoul(key.c_str(), nullptr, 10))->push_back(strtoul(value.c_str(), nullptr, 10));
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

            while (getline(streamline, t, ','))
            {
                output.push_back(stoi(t));
                cout << t << endl;
            }

            vector<int> instructionSet;

            for (int i = 1; i < output.size(); i++)
            {

                // get the vector already stored at this key
                if (instructions.find(output[i]) != instructions.end())
                {
                    for (int k = 0; k < output.size(); k++)
                    {
                        instructions.at(output[i]).get();
                    }
                }

                if (i == output.size() - 1)
                {
                    sum += output[output.size() / 2 + 1];
                }
            }

            cout << "\n"<< endl;
        }
    }
    cout << "The sum of the middle numbers is: " << sum << endl;
}