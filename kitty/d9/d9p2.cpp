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
    vector<unique_ptr<int>> IDsCopy;
    vector<unique_ptr<int>> sizes;
    vector<unique_ptr<int>> sizesCopy;
    int size = 0;
    bool isID = true;
    int id = 0;

    int processedFilelength = 0;

    for (int i = 0; i < inputText.length(); i++)
    {
        size = (int)inputText[i] - 48;
        processedFilelength += size;
        sizes.push_back(make_unique<int>(size));
        sizesCopy.push_back(make_unique<int>(size));

        if (isID)
        {
            IDs.push_back(make_unique<int>(id));
            IDsCopy.push_back(make_unique<int>(id));
            id++;
            isID = false;
        }
        else
        {
            IDs.push_back(nullptr);
            IDsCopy.push_back(nullptr);
            isID = true;
        }
    }
    cout << "IDs array: " << endl;
    for (int ID = 0; ID < IDs.size(); ID++)
    {
        if (IDs[ID].get() != nullptr)
        {
            cout << to_string(*(IDs[ID].get()));
        }
        else
        {
            cout << ".";
        }
    }

    cout << endl;
    // the final sum
    uint64_t sum = 0;
    bool keepLooking = true;
    int forwardsPos = 0;
    int additionCount = 0;

    // start with the first ID
    for (int mainIndex = IDs.size() - 1; mainIndex > 0; mainIndex--)
    {
        // cout << "Looing at index: " << mainIndex << endl;
        if (mainIndex <= forwardsPos)
        {
            break;
        }

        // get the first non-null ID
        if (IDsCopy[mainIndex].get() != nullptr)
        {
            if (*IDsCopy[mainIndex].get() == 1)
            {
            }

            cout << "\nLooking at ID: " << to_string(*IDsCopy[mainIndex].get()) << endl;

            keepLooking = true;

            while (keepLooking)
            {
                if (forwardsPos >= mainIndex - 1 + additionCount)
                {
                    // don't move anything around
                    cout << "This ID will not fit in any space." << endl;

                    forwardsPos = 0; // start looking from the beginning again for free space and move to the next ID
                    // stop looking for somewhere to put this item
                    keepLooking = false;
                }
                else
                {
                    // find the first bit of space
                    if (IDs[forwardsPos] == nullptr)
                    {
                        cout << "Size of lump to move: " << *sizesCopy[mainIndex] << endl;

                        cout << "Size of space: " << *sizes[forwardsPos] << endl;

                        // check if the size of this ID will fit
                        if (*sizesCopy[mainIndex] <= *sizes[forwardsPos])
                        {
                            cout << "Moving" << endl;
                            int sizeSpace = *sizes[forwardsPos];
                            int sizeMove = *sizesCopy[mainIndex];
                            int ID = *IDsCopy[mainIndex];

                            // Add in a null ID at the the mainIndex
                            if (sizeMove != sizeSpace)
                            {
                                cout << "null added with size " << sizeSpace - sizeMove << endl;
                                sizes.insert(sizes.begin() + forwardsPos + 1, make_unique<int>(sizeSpace - sizeMove));
                                IDs.insert(IDs.begin() + forwardsPos + 1, nullptr);

                                // put for both the sizes and the ID vectors put this ID at the forward pos index
                                // and erase it from its current position
                                additionCount++;
                            }

                            // put for both the sizes and the ID vectors put this ID at the forward pos index
                            // and erase it from its current position
                            sizes.erase(sizes.begin() + mainIndex + additionCount);
                            IDs.erase(IDs.begin() + mainIndex + additionCount);

                            sizes[forwardsPos] = make_unique<int>(sizeMove);
                            IDs[forwardsPos] = make_unique<int>(ID);

                            forwardsPos = 0; // start looking from the beginning again for free space and move to the next ID
                            keepLooking = false;
                        }
                    }

                    forwardsPos++;
                }
            }

            cout << "IDs array: " << endl;
            for (int ID = 0; ID < IDs.size(); ID++)
            {
                if (IDs[ID].get() != nullptr)
                {
                    cout << to_string(*(IDs[ID].get()));
                }
                else
                {
                    cout << ".";
                }
            }

            cout << "\nsizes array: " << endl;
            for (int size = 0; size < sizes.size(); size++)
            {
                cout << to_string(*(sizes[size].get()));
            }
            cout << endl;
        }
    }

    // make the final processed file
    vector<unique_ptr<int>> processedFile;

    for (int i = 0; i < IDs.size(); i++)
    {
        for (int s = 0; s < *sizes[i]; s++)
        {
             if (IDs[i] != nullptr)
        {
            processedFile.push_back(make_unique<int>(*IDs[i]));
        }
        else{
            processedFile.push_back(nullptr);
        }
            
        }
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