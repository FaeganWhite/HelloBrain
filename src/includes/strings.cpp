#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <vector>
using namespace std;

// Split a string into a vector
vector<string> split(string input, char splitter) {
    // Establish the output
    vector<string> output;
    // Var to store the current position in the loop
    int pos = 0;
    // var to store the last position of the splitter
    int last_pos = 0;
    // For each character in the input
    for (char c : input) {
        // If the character matches the splitter character
        if (c == splitter or pos == input.length()-1) {
            // Add the substring to the vector
            output.push_back(input.substr(last_pos, pos));
            // Update the last position
            last_pos = pos+1;
        }
        // Increment the position
        pos++;
    }
    // Return the output
    return output;
}

// Strip whitespace from string ends
string strip(string input) {
    // Establish the start
    int start_pos = 0;
    // Establish the end
    int end_pos = input.length();
    // Establish the current position
    int pos = 0;
    // For each character in the input
    for (char c : input) {
        // If the character isn't whitespace
        if (!isspace(c)) {
            // Save the position
            start_pos = pos;
            // Break the loop
            break;
        }
        // Increment the position
        pos++;
    }
    // For each character in the input, woking backwards
    for (pos = input.length()-1; pos >= 0; pos--) {
        // If the character at the position isn't whitespace
        if (!isspace(input.at(pos))) {
            // Save the position
            end_pos = pos+1-start_pos;
            // Break the loop
            break;
        }
    }
    // Return the output
    return input.substr(start_pos, end_pos);
}

// Function to print a line of text
void print(string message) {
    // Print a line of text
    cout << message << endl;
}

// Function to print a line of text
void print(int message) {
    // Print a line of text
    cout << message << endl;
}