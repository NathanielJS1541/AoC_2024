#include <regex>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    // Create a text string, which is used to output the text file
    string myText;

    // Read from the text file
    ifstream MyReadFile("input.txt");

    // Regular expression for two multiples
    regex rgx("mul\\((\\d*),(\\d*)\\)"); 

    // variable to hold the captured groups
    smatch matches;

    int sum = 0;

    // Use a while loop together with the getline() function to read the file line by line
    while (getline(MyReadFile, myText))
    {
        // Output the text from the file
        regex_search(myText, matches, rgx);

        // iterator to go through the string
        string::const_iterator searchStart(myText.cbegin());

        // Iterate over the string
        while (regex_search(searchStart, myText.cend(), matches, rgx))
        {
            // print out the given matches
            for (size_t i = 0; i < matches.size(); ++i)
            {
                cout << "MATCH: " << matches[i] << endl;
            }

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
