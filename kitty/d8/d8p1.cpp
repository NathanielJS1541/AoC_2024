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
    ifstream inputFile("test.txt");
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

    for (int row = 0; row < matrixHeight; row++)
    {
        for (int col = 0; col < matrixWidth; col++)
        {
            if (inputText[row * matrixWidth + col] != '.')
            {
                if (count(characters.begin(), characters.end(), inputText[row * matrixWidth + col]) == 0)
                {
                    characters.push_back(inputText[row * matrixWidth + col]);
                }
            }
        }
    }

    cout << "Characters: ";

    vector<array<int, 2>> indexes;

    vector<array<int, 2>> antinodes;

    array<int, 2> node{0, 0};

    // go through each char and make a list of the antinodes
    for (int charac = 0; charac < characters.size(); charac++)
    {
        indexes.clear();

        cout << "This character is: " << characters[charac] << endl;

        // go through the array and keep a list of where all the 'r's are.
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

        cout << "Which appears at" << endl;

        for (auto i : indexes)
            cout << "Row: " << i[0] << " Col: " << i[1] << ' ' << endl;

        cout << "\n";
        // go through the array at each index and find the halfway point between that index and all the others
        // keep a list of these "anitnodes" where they are still in the original array

        // start at the beginning and look at all following indeces
        for (int i = 0; i < indexes.size() - 1; i++)
        {

            cout << "The first index is: " << indexes[i][0] << ", " << indexes[i][1] << endl;
            cout << "The second index is: " << indexes[i+1][0] << ", " << indexes[i+1][1] << endl;

            // index closer to i is:
            node[0] = abs(indexes[i][0] + (indexes[i][0] - indexes[i + 1][0]) / 3);
            node[1] = abs(indexes[i][1] + (indexes[i][1] - indexes[i + 1][1]) / 3);

            // check if this node is in bounds
            if (((0 < node[0]) && (node[0] < matrixHeight)) && ((0 < node[1]) && (node[1] < matrixWidth)))
            {
                if (count(antinodes.begin(), antinodes.end(), node) == 0)
                {
                    antinodes.push_back(node);

                    cout << "The node is: " << node[0] << ", " << node[1] << endl;
                }
            }

            // index closer to the next index is:
            node[0] = abs(indexes[i+1][0] - (indexes[i][0] - indexes[i + 1][0]) / 3);
            node[1] = abs(indexes[i+1][1] - (indexes[i][1] - indexes[i + 1][1]) / 3);

            // check if this node is in bounds
            if (((0 < node[0]) && (node[0] < matrixHeight)) && ((0 < node[1]) && (node[1] < matrixWidth)))
            {
                if (count(antinodes.begin(), antinodes.end(), node) == 0)
                {
                    antinodes.push_back(node);

                    cout << "The node is: ";
                    for (auto i : node)
                        cout << i;
                    cout << "\n";
                }
            }
        }
    }

    cout << "\n"
         << endl;

    cout << "The number of unique antinodes is: " << antinodes.size() << endl;
}