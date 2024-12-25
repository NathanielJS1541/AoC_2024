#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <cstdlib>
#include <algorithm>

using namespace std;

// Structure to represent a file with its ID, start position, and length
struct File {
   int fileID;
   int start;
   int length;
};

// Function to parse the disk map and convert it to block representation
vector<int> parseDiskMap(const string& diskMap, vector<File>& files) {
   vector<int> blocks;
   int fileID = 0;
   bool isFile = true; // Start with file length

   int currentPosition = 0; // To track the start position of each file

   for (size_t i = 0; i < diskMap.size(); ++i) {
       char c = diskMap[i];
       if (!isdigit(c)) {
           cerr << "Invalid character in disk map: " << c << endl;
           exit(1);
       }
       int length = c - '0';
       if (isFile) {
           if (length > 0) { // Only consider files with length > 0
               // Record the file's details
               File file;
               file.fileID = fileID;
               file.start = currentPosition;
               file.length = length;
               files.push_back(file);
               fileID++;
           }
           for (int j = 0; j < length; ++j) {
               blocks.push_back(fileID - 1); // Assign fileID to each block
               currentPosition++;
           }
       }
       else {
           for (int j = 0; j < length; ++j) {
               blocks.push_back(-1); // -1 represents free space
               currentPosition++;
           }
       }
       isFile = !isFile; // Alternate between file and free space
   }

   return blocks;
}

// Function to compact the disk by shifting file blocks one at a time from end to left (Part One)
void compactDiskPartOne(vector<int>& blocks) {
   int n = blocks.size();

   while (true) {
       // Find the leftmost free space
       int leftFree = -1;
       for (int i = 0; i < n; ++i) {
           if (blocks[i] == -1) {
               leftFree = i;
               break;
           }
       }

       if (leftFree == -1) {
           // No free spaces left; disk is fully compacted
           break;
       }

       // Find the rightmost file block after the leftFree
       int rightFile = -1;
       for (int i = n - 1; i > leftFree; --i) {
           if (blocks[i] != -1) {
               rightFile = i;
               break;
           }
       }

       if (rightFile == -1) {
           // No file blocks to move; compaction complete
           break;
       }

       // Move the file block to the leftmost free space
       blocks[leftFree] = blocks[rightFile];
       blocks[rightFile] = -1;
   }
}

// Function to calculate the checksum
long long calculateChecksum(const vector<int>& blocks) {
   long long checksum = 0;
   for (size_t pos = 0; pos < blocks.size(); ++pos) {
       if (blocks[pos] != -1) { // If it's a file block
           checksum += static_cast<long long>(pos) * blocks[pos];
       }
   }
   return checksum;
}

// Function to display the disk blocks (for debugging purposes)
void displayDisk(const vector<int>& blocks) {
   for (size_t i = 0; i < blocks.size(); ++i) {
       if (blocks[i] == -1)
           cout << ".";
       else
           cout << blocks[i];
   }
   cout << endl;
}

// Function to compact the disk by moving entire files to the leftmost suitable free space (Part Two)
void compactDiskPartTwo(vector<int>& blocks, vector<File>& files) {
   // Sort files in order of decreasing fileID
   sort(files.begin(), files.end(), [](const File& a, const File& b) -> bool {
       return a.fileID > b.fileID;
       });

   for (auto& file : files) {
       // Skip files with length 0
       if (file.length == 0) continue;

       // Find the current start of the file
       int currentStart = -1;
       for (size_t i = 0; i < blocks.size(); ++i) {
           if (blocks[i] == file.fileID) {
               currentStart = i;
               break;
           }
       }

       if (currentStart == -1) {
           // File not found; possibly already moved
           continue;
       }

       // Find the leftmost span of free space that can fit the file and is to the left of the current position
       int bestStart = -1;
       for (int i = 0; i <= currentStart - file.length; ++i) {
           bool canFit = true;
           for (int j = 0; j < file.length; ++j) {
               if (blocks[i + j] != -1) {
                   canFit = false;
                   break;
               }
           }
           if (canFit) {
               bestStart = i;
               break; // Found the leftmost suitable span
           }
       }

       if (bestStart == -1) {
           // No suitable span found; do not move the file
           continue;
       }

       if (bestStart == currentStart) {
           // File is already in the correct position
           continue;
       }

       // Move the entire file to bestStart
       for (int j = 0; j < file.length; ++j) {
           blocks[bestStart + j] = file.fileID;
           blocks[currentStart + j] = -1;
       }

       // Update the file's start position
       file.start = bestStart;
   }
}

int main() {
   string diskMap;
   ifstream inputFile("input09.txt"); // Open the file input08.txt

   if (!inputFile) {
       cerr << "Error: Unable to open file input08.txt" << endl;
       return 1;
   }

   // Read the disk map as a single line input
   // Assuming the disk map is on the first line of the file
   getline(inputFile, diskMap);
   inputFile.close(); // Close the file after reading

   if (diskMap.empty()) {
       cerr << "Error: Disk map is empty in input08.txt" << endl;
       return 1;
   }

   // Part One: Parse the disk map and perform compaction
   vector<File> filesPartOne;
   vector<int> blocksPartOne = parseDiskMap(diskMap, filesPartOne);

   // Debug: Print parsed blocks for Part One
   /*
   cout << "Parsed Blocks (Part One):" << endl;
   displayDisk(blocksPartOne);
   */

   // Compact the disk (Part One)
   compactDiskPartOne(blocksPartOne);

   // Debug: Print compacted blocks for Part One
   /*
   cout << "Compacted Blocks (Part One):" << endl;
   displayDisk(blocksPartOne);
   */

   // Calculate the checksum (Part One)
   long long checksumPartOne = calculateChecksum(blocksPartOne);

   // Output the checksum for Part One
   cout << checksumPartOne << endl;

   // Part Two: Parse the disk map again to reset the blocks and files
   vector<File> filesPartTwo;
   vector<int> blocksPartTwo = parseDiskMap(diskMap, filesPartTwo);

   // Compact the disk (Part Two)
   compactDiskPartTwo(blocksPartTwo, filesPartTwo);

   // Debug: Print compacted blocks for Part Two
   /*
   cout << "Compacted Blocks (Part Two):" << endl;
   displayDisk(blocksPartTwo);
   */

   // Calculate the checksum (Part Two)
   long long checksumPartTwo = calculateChecksum(blocksPartTwo);

   // Output the checksum for Part Two
   cout << checksumPartTwo << endl;

   return 0;
}