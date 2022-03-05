#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <vector>
using namespace std;

// Split a string into a vector of strings
vector<string> split(string input, string delimiter) {
    // Establish the output
    vector<string> output;
    // Set the start position
    size_t position = 0;
    // Establish a token string
    string token;
    // While the delimiter is present in the string
    while ((position = input.find(delimiter)) != string::npos) {
        // Get the token as the substring before the delimiter
        token = input.substr(0, position);
        // Add the token to the output vector
        output.push_back(token);
        // Erase the token from the inpuy
        input.erase(0, position + delimiter.length());
    }
    // Add the final token to the output
    output.push_back(input);
    // Return the outpu
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

// Strip whitespace from a vector of strings
vector<string> strip_vector(vector<string> input) {
    // Establish the output
    vector<string> output;
    // For each string
    for (string s : input) {
        // Add the stripped string to the output
        output.push_back(strip(s));
    }
    // Return the output
    return output;
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

// Function to print a line of text
void print(double message) {
    // Print a line of text
    cout << message << endl;
}