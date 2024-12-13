#include <regex>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{

    // Read from the text file
    ifstream MyReadFile("input.txt");

    // Regular expression for two multiples
    regex rgxMult("mul\\((\\d*),(\\d*)\\)");

    // Regular expression for finding set between do and dont
    regex rgxDo("do\\(\\)(.*?)don't\\(\\)");

    

    // variable to hold the captured groups
    smatch matchesMult;
    smatch matchesDo;

    int sum = 0;

    // Read the whole file into a string
    string myText((istreambuf_iterator<char>(MyReadFile)),
                  istreambuf_iterator<char>());

    myText.insert(0, "do()");

    myText.insert(myText.length() - 1, "don't()");

    //remove anynewlines
    myText.erase(remove(myText.begin(), myText.end(), '\n'), myText.cend());

    //print the string we are searching in
    // cout << "Full Text :" << "\n"
    //      << myText << "\n"
    //      << endl;

    // iterator to go through the string
    string::const_iterator searchStartMain(myText.cbegin());

    string searchText;

    while (regex_search(searchStartMain, myText.cend(), matchesDo, rgxDo))
    {
        // set the string to be only the text after the match
        searchStartMain = matchesDo.suffix().first;

        // // print the remaining string
        // size_t index = distance(myText.cbegin(), searchStartMain);

        // cout << "Remaining text: ";
        // for (size_t i = index; i < myText.length(); i++)
        // {
        //     cout << myText[i];
        // }

        // cout << "\n";

        searchText = matchesDo[1].str();

        // // print the string we are searching in
        // cout << "The search text: ";
        // cout << matchesDo.str() << endl;

        // cout << "\n";

        // iterator to go through the string
        string::const_iterator searchStart(searchText.cbegin());

        // variable to hold the captured groups
        smatch matches;

        // print out the given matchesMult
        // Iterate over the string
        while (regex_search(searchStart, searchText.cend(), matches, rgxMult))
        {
            // print out the given matches
            // cout << "MATCH: " << matches[0] << endl;

            // multiply the matches together
            sum += (stoi(matches[1]) * stoi(matches[2]));

            // set the string to be only the text after the match
            searchStart = matches.suffix().first;
        }
    }

    // Close the file
    MyReadFile.close();

    cout << "The sum is: " << sum << endl;
}