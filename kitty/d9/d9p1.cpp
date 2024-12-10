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
    ifstream inputFile("input.txt");
    string inputText;

    // each char files ... free space ... files ... free space ... files ........ etc
    // with id   0 ...................... 1 ...................... 2 ............ etc

    // move each number from the end to the next free space (starting from the end)

    // output position * number there

    // read the whole array into a string
    getline(inputFile, inputText);

    cout << "Original File: " << inputText <<endl;

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
            processedFile.insert(processedFile.end(), size, make_unique<int>(id));
            id++;
            isID = false;
        }
        else
        {
            processedFile.insert(processedFile.end(), size, make_unique<int>(nullptr));
            isID = true;
        }
    }

    for (auto item =0; item < processedFile.size(); item++){
        cout << processedFile[item].get();
    }
    cout << endl;

    int *finalInt;
    int fileLength = processedFile.size();
    int sum = 0;
    int currentPosition = 0;

    // starting from the end move the final ID to the first free space
    for (int finalIntInd = fileLength-1; finalIntInd >= 0; finalIntInd--)
    {
        
        // if the cross over point have been reached between reading back down the string for 
        // characters to move and reading up the array for free spaces then break
        if (currentPosition>finalIntInd-1){
            break;
        }

        finalInt = processedFile[finalIntInd].get();
        
        // cout << "\nThe new processed file: " << processedFile<<endl;

        // cout<< "This final char: "<<finalInt <<endl;
        // cout<< "The current position: "<<currentPosition <<endl;

        if (finalInt != nullptr)
        {
            for (int nextFree = currentPosition; nextFree <= fileLength; nextFree++)
            {
                // cout<< "This number being checked for .: "<<processedFile[nextFree] <<endl;

                if(processedFile[nextFree].get() == nullptr){
                    currentPosition = nextFree + 1;

                    *processedFile[nextFree].get() = *finalInt;
                    processedFile[finalIntInd] = nullptr;
                    break;
                }

            }
        }
    
    }

    
    cout<< "\nThe final processed file is: "<< endl;
    for (auto item =0; item < processedFile.size(); item++){
        cout << processedFile[item].get();
    }
    cout << endl;

    for(int i = 0; i < processedFile.size(); i++){
        if (processedFile[i].get() == nullptr){
            break;
        }
        // cout<< i<<"*"<< processedFile[i]<<endl;
        sum += i*(*processedFile[i].get());
    }

    cout<< "\nThe sum is: "<<sum <<endl;
}