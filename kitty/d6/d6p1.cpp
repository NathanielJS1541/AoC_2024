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
    string file = "input.txt";
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

    matrixWidth = matrixWidth + 2;
    matrixHeight = matrixHeight + 2;

    cout << "Full Matrix: " << endl;

    for (int i = 0; i < matrixHeight; i++)
    {
        for (int j = 0; j < matrixWidth; j++)
        {
            cout << fullText[i * (matrixWidth) + j];
        }

        cout << "\n";
    }

    // find the initial position of the guard
    int gaurdPos[] = {0, 0};
    int gaurdPosNext[] = {0, 0};

    for (int row = 0; row < matrixHeight; row++)
    {
        for (int col = 0; col < matrixWidth; col++)
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
        if (fullText[gaurdPosNext[0] * (matrixWidth) + gaurdPosNext[1]] == '#')
        {
            // reset the guard position
            gaurdPosNext[0] = gaurdPos[0];
            gaurdPosNext[1] = gaurdPos[1];

            state = nextState;
        }
        // check if the guard leaves the map
        else if (fullText[gaurdPos[0] * (matrixWidth) + gaurdPos[1]] == '0')
        {
            cont = false;
        }
        // if not add another position to the sum
        else
        {
            if (!usedPositions[gaurdPos[0] * (matrixWidth) + gaurdPos[1]])
            {
                usedPositions[gaurdPos[0] * (matrixWidth) + gaurdPos[1]] = true;
                sum ++;
            }
            copy(gaurdPosNext, gaurdPosNext + 2, gaurdPos);
        }
    }

    cout << "The sum is: " << sum << endl;
}