#include <string>
#include <vector>
#include <fstream>
#include "board_shim.h"
using namespace std;

// Read a text file as a vector
vector<string> readFile(string location) {
    // Create the output string variable
    vector<string> output;
    // Create a variable to hold a line
    string line;
    // Open the file at the given location
    ifstream File(location);
    // For each line
    while (getline(File, line)) {
        // Add it to the output
        output.push_back(line);
    }
    // Close the file
    File.close();
    // Return the output
    return output;
}

// Save text to a file
void saveFile(string location, string text) {
    // Create the file at the given location
    ofstream File(location);
    // Add the text to the file
    File << text;
    // Close the file
    File.close();
}

// Save text to a file
void saveFile(string location, BrainFlowArray<double, 2> text) {
    // Create the file at the given location
    ofstream File(location);
    // Add the text to the file
    File << text;
    // Close the file
    File.close();
}