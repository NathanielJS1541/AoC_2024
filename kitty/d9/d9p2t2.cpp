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
    // open the file and read it
    ifstream inputFile("test.txt");
    string inputText;

    // each char files ... free space ... files ... free space ... files ........ etc
    // with id   0 ...................... 1 ...................... 2 ............ etc

    // move each number from the end to the next free space (starting from the end)

    // output position * id

    // read the whole array into a string
    getline(inputFile, inputText);

    // cout << "Original File: " << inputText << endl;

    // convert to ids and numbers
    vector<unique_ptr<int>> processedFile;

    int size = 0;
    bool isID = true;
    int id = 0;

    for (int i = 0; i < inputText.length(); i++)
    {
        size = (int)inputText[i] - 48;

        if (isID)
        {
            for (int k = 0; k < size; k++)
            {
                processedFile.push_back(make_unique<int>(id));
            }

            id++;
            isID = false;
        }
        else
        {
            for (int k = 0; k < size; k++)
            {
                processedFile.push_back(nullptr);
            }
            isID = true;
        }
    }

    for (auto item = 0; item < processedFile.size(); item++)
    {
        if (processedFile[item].get() != nullptr)
        {
            cout << to_string(*processedFile[item].get());
        }
        else
        {
            cout << '.';
        }
    }
    cout << endl;

    int *finalInt;
    int *finalIntPrev;
    int fileLength = processedFile.size();
    uint64_t sum = 0;
    int currentPosition = 0;

    // starting from the end move the final ID to the first free space
    for (int finalIntInd = fileLength - 1; finalIntInd >= 0; finalIntInd--)
    {

        // if the cross over point have been reached between reading back down the string for
        // characters to move and reading up the array for free spaces then break
        if (currentPosition >= finalIntInd)
        {
            break;
        }

        // vector<unique_ptr<int>> duplicateProcessedFile(processedFile);

        finalInt = processedFile[finalIntInd].get();

        cout << "\nThis int to move: " << *finalInt << endl;

        cout << "The new processed file is: " << endl;

        for (auto item = 0; item < processedFile.size(); item++)
        {
            if (processedFile[item].get() != nullptr)
            {
                cout << to_string(*processedFile[item].get()) << " ";
            }
            else
            {
                cout << ". ";
            }
        }
        cout << endl;

        if (finalInt != nullptr)
        {
            if (finalIntPrev == nullptr || *finalInt != *finalIntPrev)
            {
                for (int nextFree = currentPosition; nextFree <= fileLength; nextFree++)
                {
                    if (processedFile[nextFree].get() == nullptr)
                    {

                        cout << "The space at index : " << nextFree  << " will be filled with: " << *finalInt << endl;

                        // if going to move the next number too then check this is still in the same block
                        // do not move if the next space is not empty but the number to move is in the same block
                        if ((processedFile[nextFree + 1].get() != nullptr) && (*processedFile[finalIntInd].get() == *processedFile[finalIntInd - 1].get()))
                        {
                            continue;
                        }
                        // also do not move to the next number if the next space is empty but the next number to move is not the same.
                        else if ((processedFile[nextFree + 1].get() == nullptr) && (processedFile[finalIntInd - 1].get() != nullptr) && (*finalInt != *processedFile[finalIntInd - 1].get()))
                        {
                            processedFile[nextFree] = make_unique<int>(*finalInt);
                            processedFile[finalIntInd] = nullptr;
                            // do not undo that changes in this case
                            break;
                        }
                        else
                        {
                            processedFile[nextFree] = make_unique<int>(*finalInt);
                            processedFile[finalIntInd] = nullptr;
                            currentPosition = nextFree + 1;
                            cout << "The next space is free " << nextFree + 1 << " will be filled with: " << *(processedFile[finalIntInd - 1].get()) << endl;
                            break;
                        }

                        processedFile[finalIntInd] = nullptr;
                    }
                }
            }
            else
            {
                finalIntPrev = nullptr;
            }
        }
    }
    // output the final array
    cout << "\nThe final processed file is: " << endl;
    for (auto item = 0; item < processedFile.size(); item++)
    {
        if (processedFile[item].get() != nullptr)
        {
            cout << to_string(*processedFile[item].get()) << " ";
        }
        else
        {
            cout << '.';
        }
    }
    cout << endl;

    // calculate the sum
    for (int i = 0; i < processedFile.size(); i++)
    {
        if (processedFile[i].get() == nullptr)
        {
            break;
        }
        // cout<< sum << endl;
        sum += i * (*processedFile[i].get());
    }

    cout << "\nThe sum is: " << sum << endl;
}