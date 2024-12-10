#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <array>

using namespace std;

int main()
{
    // open the file and read it
    ifstream inputFile("input.txt");
    string line;
    string inputText;
    int matrixWidth;
    int matrixHeight = 1;

    getline(inputFile, line);
    matrixWidth = line.length();
    inputText.append(line);

    while (getline(inputFile, line))
    {
        inputText.append(line);
        matrixHeight++;
    }

    // loop through the matrix and find all the unique characters
    vector<char> characters;
    int charCount = 0;

    for (int row = 0; row < matrixHeight; row++)
    {
        for (int col = 0; col < matrixWidth; col++)
        {
            // if the character is not an antenna (for test) and not a '.' (the blank space char then count this as a unique character)
            if ((inputText[row * matrixWidth + col] != '.') && (inputText[row * matrixWidth + col] != '#'))
            {
                // each character is an antenna now so count this
                charCount++;

                // make sure this chacacter has not already been counted
                if (count(characters.begin(), characters.end(), inputText[row * matrixWidth + col]) == 0)
                {
                    characters.push_back(inputText[row * matrixWidth + col]);
                }
            }
        }
    }

    // a vector holding the index (row, col) of the positions of this character
    vector<array<int, 2>> indexes;

    // a string to hold a visual map of where all the antennas are
    string antennaMap = inputText;

    // count of the individual nodes
    int nodeCount = 0;

    // the current node we are looking at
    array<int, 2> node{0, 0};

    // go through each char and make a list of the antinodes
    for (int charac = 0; charac < characters.size(); charac++)
    {
        indexes.clear();

        // cout << "This character is: " << characters[charac] << endl;

        // go through the array and keep a list of where this character appears.
        for (int row = 0; row < matrixHeight; row++)
        {
            for (int col = 0; col < matrixWidth; col++)
            {
                if (inputText[row * matrixWidth + col] == characters[charac])
                {
                    indexes.push_back({row, col});
                }
            }
        }

        // cout << "Which appears at" << endl;

        // for (auto i : indexes)
        //     cout << "Row: " << i[0] << " Col: " << i[1] << ' ' << endl;

        // cout << "\n";

        // go through the array at each index and find the antenna on the opposite side
        for (auto firstIndex : indexes)
        {
            // compare this index to all other indexes
            for (auto secondIndex : indexes)
            {
                // if we are not looking at the same index for both characters
                if (firstIndex != secondIndex)
                {
                    int nodesfrompair = 1;

                    // keep adding antennas in the given direction until the the node becomes out of range 
                    while (((0 <= node[0]) && (node[0] < matrixHeight)) && ((0 <= node[1]) && (node[1] < matrixWidth)))
                    {
                        // index closer to i is:
                        node[0] = firstIndex[0] + (firstIndex[0] - secondIndex[0]) * nodesfrompair;
                        node[1] = firstIndex[1] + (firstIndex[1] - secondIndex[1]) * nodesfrompair;

                        // cout << "Attempted node: " << node[0] << ", " << node[1] << endl;

                        // check if this node is in bounds
                        if (((0 <= node[0]) && (node[0] < matrixHeight)) && ((0 <= node[1]) && (node[1] < matrixWidth)))
                        {
                            // check if the node has not already been counted, this time as the characters have already been counted check
                            // that this is a '.' blank space
                            if (antennaMap[node[0] * matrixWidth + node[1]] != '#' && antennaMap[node[0] * matrixWidth + node[1]] == '.')
                            {
                                nodeCount++;
                                antennaMap[node[0] * matrixWidth + node[1]] = '#';
                            }
                        }

                        nodesfrompair++;
                    }
                }
                // reset the node
                node[0] = 0;
                node[1] = 0;
            }
        }

        // print the map of the antennas
        cout << "Output antenna map: " << endl;

        for (int row = 0; row < matrixHeight; row++)
        {
            for (int col = 0; col < matrixWidth; col++)
            {
                cout << antennaMap[row * matrixWidth + col];
            }
            cout << "\n";
        }
    }

    cout << "\n"
         << endl;

    cout << "The number of unique antinodes is: " << nodeCount + charCount << endl;
}