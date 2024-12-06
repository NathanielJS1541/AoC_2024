#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define kernelSize 7      // NxN size of the kernel and window
#define requiredMatches 4 // number of matches required between kernel and window

using namespace std;

int main()
{
    // define file location string
    string file = "test.txt";
    string line;
    string fullText;

    // Read from the text file
    ifstream myReadFile(file);

    // get array width
    getline(myReadFile, line);
    int matrixWidth = line.length();

    // add a boundary to the map
    fullText.insert(fullText.begin(), matrixWidth + 2, '0');

    fullText.append("0" + line + "0");

    int matrixHeight = 1;
    // get array height and append into file
    while (getline(myReadFile, line))
    {
        // add the line itself
        fullText.append("0" + line + "0");

        // make note of the matrix height
        matrixHeight++;
    }

    // add a boundary to the map
    fullText.insert(fullText.end(), matrixWidth + 2, '0');

    cout << "Full Matrix: " << endl;

    for (int i = 0; i < matrixHeight + 2; i++)
    {
        for (int j = 0; j < matrixWidth + 2; j++)
        {
            cout << fullText[i * (matrixWidth + 2) + j];
        }

        cout << "\n";
    }

    // find the initial position of the guard
    int gaurdPos[] = {0, 0};
    int gaurdPosNext[] = {0, 0};

    for (int row = 1; row < matrixHeight+1; row++)
    {
        for (int col = 1; col < matrixWidth+1; col++)
        {
            if (fullText[(matrixWidth)*row + col] == '^')
            {
                gaurdPos[0] = row;
                gaurdPos[1] = col;
                gaurdPosNext[0] = row;
                gaurdPosNext[1] = col;

                break;
            }
        }
    }

    bool cont = true;

    enum Direction
    {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };

    Direction state = UP;
    Direction nextState = UP;

    // create a vector to store all the used positions
    vector<bool> usedPositions(fullText.length());

    int sum = 0;
    int stateCount = 0; // the guard must have entered each state at least once for a loop to occur. This gets reset if a new position is used (a new path has been taken)
    int loopCount = 0;

    char prevChar = '0';

    // iterate over every posible position number and check if sum has not increased when all states completed.
    // Not including the boundaries
    for (int row = 1; row < matrixHeight + 1; row++)
    {
        for (int col = 0; col < matrixWidth + 1; col++)
        {
            // set the obsicle position- if there wasnt one one there already
            if (fullText[(row) * (matrixWidth + 2) + col] != '#')
            {
                prevChar = fullText[row * (matrixWidth + 2) + col];

                fullText[row * (matrixWidth + 2) + col] = '#';
                
                cout << "Full Matrix: " << endl;

                for (int i = 1; i < matrixHeight + 1; i++)
                {
                    for (int j = 1; j < matrixWidth + 1; j++)
                    {
                        cout << fullText[i * (matrixWidth + 2) + j];
                    }

                    cout << "\n";
                }
                stateCount = 0;

                while (cont)
                {
                    switch (state)
                    {
                    case UP:

                        // move up one row
                        gaurdPosNext[0] = gaurdPos[0] - 1;
                        nextState = RIGHT;
                        break;

                    case RIGHT:
                        // move right one col
                        gaurdPosNext[1] = gaurdPos[1] + 1;
                        nextState = DOWN;
                        break;

                    case DOWN:
                        // move down one row
                        gaurdPosNext[0] = gaurdPos[0] + 1;
                        nextState = LEFT;
                        break;

                    case LEFT:
                        // move left one row
                        gaurdPosNext[1] = gaurdPos[1] - 1;
                        nextState = UP;
                        break;
                    }

                    // check if a barrier was hit
                    if (fullText[gaurdPosNext[0] * (matrixWidth + 2) + gaurdPosNext[1]] == '#')
                    {
                        // reset the guard position
                        gaurdPosNext[0] = gaurdPos[0];
                        gaurdPosNext[1] = gaurdPos[1];

                        state = nextState;
                        stateCount++;
                    }
                    // check if the guard leaves the map
                    else if (fullText[gaurdPos[0] * (matrixWidth + 2) + gaurdPos[1]] == '0')
                    {
                        cont = false;
                    }
                    // if not add another position to the sum
                    else
                    {
                        if (!usedPositions[gaurdPos[0] * (matrixWidth + 2) + gaurdPos[1]])
                        {
                            usedPositions[gaurdPos[0] * (matrixWidth + 2) + gaurdPos[1]] = true;
                            stateCount = 0;
                        }

                        copy(gaurdPosNext, gaurdPosNext + 2, gaurdPos);
                    }

                    if (stateCount == 3)
                    {
                        loopCount++;
                        continue;
                    }
                }
                
                // reset the array
                fullText[row * (matrixWidth + 2) + col] = prevChar;
            }
        }
    }

    cout << "The loop number is: " << loopCount << endl;
}