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

    // move a full block of IDs at a time and if the current block won't fit then move
    // to the next one.

    // output position * id

    // read the whole array into a string
    getline(inputFile, inputText);

    // cout << "Original File: " << inputText << endl;

    // switching to two arrays
    //  - one of IDs
    //  - one of sizes

    vector<unique_ptr<int>> IDs;
    vector<unique_ptr<int>> sizes;
    int size = 0;
    bool isID = true;
    int id = 0;

    int processedFilelength = 0;

    for (int i = 0; i < inputText.length(); i++)
    {
        size = (int)inputText[i] - 48;
        processedFilelength += size;
        sizes.push_back(make_unique<int>(size));

        if (isID)
        {
            IDs.push_back(make_unique<int>(id));
            id++;
            isID = false;
        }
        else
        {
            IDs.push_back(nullptr);
            isID = true;
        }
    }

    // the final sum
    uint64_t sum = 0;
    int decreaser = IDs.size() - 1;
    bool keepLooking = true;
    int backwardsPos = IDs.size() - 1;

    // the final full processed array
    vector<unique_ptr<int>> processedFile;

    for (int mainIndex = 0; mainIndex < IDs.size(); mainIndex++)
    {
        if (mainIndex >= backwardsPos)
        {
            break;
        }

        // if you have encountered a bit of free space
        // go backwards through the array until you find a lump of IDs
        // that will fit
        if (IDs[mainIndex].get() == nullptr)
        {
            keepLooking = true;
            decreaser = backwardsPos;
            while (keepLooking)
            {
                if (decreaser <= mainIndex - 1)
                {

                    cout << "Nothing fit in this space." << endl;
                    // there where no IDs that fit in this block so fill it with nulls
                    for (int l = 0; l < *sizes[mainIndex]; l++)
                    {
                        processedFile.push_back(nullptr);
                    }

                    keepLooking = false;
                }
                else
                {
                    // find the last non null ID
                    if (IDs[decreaser] != nullptr)
                    {
                        cout << "Looking at ID: " << *IDs[decreaser] << endl;

                        cout << "Size of lump to move: " << *sizes[decreaser] << endl;
                        cout << "Size of space: " << *sizes[mainIndex] << endl;

                        // check if the size of this ID will fit
                        if (*sizes[decreaser] <= *sizes[mainIndex])
                        {
                            backwardsPos = decreaser - 1;
                            cout << "Moving" << endl;
                            keepLooking = false;
                            mainIndex=0; // start looking from the beginning again
                            for (int l = 0; l < *sizes[mainIndex]; l++)
                            {
                                if (l < *sizes[decreaser])
                                {
                                    processedFile.push_back(make_unique<int>(*IDs[decreaser]));
                                }
                                // add in some nulls until the size of
                                else
                                {
                                    processedFile.push_back(nullptr);
                                }
                            }
                        }
                    }
                    decreaser--;
                }
            }
        }
        else
        {
            cout << "Looking at ID: " << *IDs[mainIndex] << endl;
            cout << "with size: " << *sizes[mainIndex] << endl;
            // If the ID is not a blank space then append the correct number of IDs to the processed file
            for (int k = 0; k < *sizes[mainIndex].get(); k++)
            {
                processedFile.push_back(make_unique<int>(*IDs[mainIndex].get()));
            }
        }

        cout << "\nThe new processed file is: " << endl;
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
    }

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

    // Calculate the sum
    for (int i = 0; i < processedFile.size(); i++)
    {
        if (processedFile[i].get() != nullptr)
        {
            sum += *processedFile[i].get() * i;
        }
    }

    cout << "The sum is: " << sum;
}