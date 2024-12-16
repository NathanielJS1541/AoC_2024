#include <iostream>
#include <fstream>
#include <string>

#define kernelSize 3 //NxN size of the kernel and window 
#define requiredMatches 5 // number of matches required between kernel and window

using namespace std;

int applyKernel(string kernel, string window)
{
    int sum = 0;
    // cout << "\nWindow: " << endl;

    // for (int i = 0; i < kernelSize; i++)
    // {
    //     for (int j = 0; j < kernelSize; j++)
    //     {
    //         cout << window[i * kernelSize + j];
    //     }
    //     cout << "\n";
    // }

    // cout << "\nKernel: " << endl;

    // for (int i = 0; i < kernelSize; i++)
    // {
    //     for (int j = 0; j < kernelSize; j++)
    //     {
    //         cout << kernel[i * kernelSize + j];
    //     }
    //     cout << "\n";
    // }

    // apply the kernel and search for matches
    for (int row = 0; row < kernelSize; row++)
    {
        for (int col = 0; col < kernelSize; col++)
        {
            if (window[kernelSize * row + col] == kernel[kernelSize * row + col])
            {
                sum++;
            }
        }
    }
    if (sum == requiredMatches)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{
    //define file location string
    string file = "input.txt";

    // Create a text string, which is used to output the text file
    string line;
    string myText = "";

    // Read from the text file
    ifstream testMyReadFile(file);

    // work out the width of the matrix
    getline(testMyReadFile, line);
    int matrixWidth = line.length();
    testMyReadFile.close();

    // Start reading from the text file again
    ifstream myReadFile(file);
    int matrixHeight = 1;

    // add three rows of zero at the beginning
    for (int j = 0; j < kernelSize/2; j++)
    {
        for (int i = 0; i < matrixWidth + kernelSize - 1; i++)
        {
            myText.append("Z");
        }
    }

    while (getline(myReadFile, line))
    {
        /// add three columns of zero at start
        myText.append("Z");
        // add the line itself
        myText.append(line);
        /// add three columns of zero at end
        myText.append("Z");

        // make note of the matrix height
        matrixHeight++;
    }

    // add three rows of zero at the end
    for (int j = 0; j < kernelSize/2; j++)
    {
        for (int i = 0; i < matrixWidth + kernelSize - 1; i++)
        {
            myText.append("Z");
        }
    }

    // cout << myText << endl;
    cout << "Full Matrix: " << endl;

    for (int i = 0; i < matrixHeight + kernelSize - 1; i++)
    {
        for (int j = 0; j < matrixWidth + kernelSize - 1; j++)
        {
            cout << myText[i * (matrixWidth + kernelSize - 1) + j];
        }

        cout << "\n";
    }

    // define the window - piece of the matrix to look at
    string window =         "000000000";

    // Define the kernel for an X shape
    string crossKernel =    "M0S0A0M0S";
    string crossKernel2 =   "M0M0A0S0S";

    // setup the sum for the number of matches
    int sum = 0;
    
    // go through the matrix row by row
    for (int rowMain = kernelSize/2; rowMain < (matrixHeight + kernelSize/2); rowMain++)
    {
        for (int colMain = kernelSize/2; colMain < (matrixWidth + kernelSize/2); colMain++)
        {
            // only bother filtering if the middle char is A
            if (myText[((matrixWidth + kernelSize - 1) * (rowMain) + (colMain))] == 'A')
            {
                // Make the window (the main letter and then 3 surrounding letters)
                for (int kernRow = 0; kernRow < kernelSize; kernRow++)
                {
                    for (int kernCol = 0; kernCol < kernelSize; kernCol++)
                    {
                        window[kernRow * kernelSize + kernCol] = myText[((matrixWidth + kernelSize - 1) * (rowMain + kernRow - kernelSize/2) + (colMain + kernCol - kernelSize/2))];
                    }
                }

                // apply the kernel
                if (applyKernel(crossKernel, window))
                {
                    sum++;
                }
                // apply the kernel
                if (applyKernel(string(crossKernel.rbegin(), crossKernel.rend()), window))
                {
                    sum++;
                }
                // apply the kernel
                if (applyKernel(crossKernel2, window))
                {
                    sum++;
                }
                // apply the kernel
                if (applyKernel(string(crossKernel2.rbegin(), crossKernel2.rend()), window))
                {
                    sum++;
                }
            }
        }
    }

    cout << "The sum is: " << sum << endl;
}