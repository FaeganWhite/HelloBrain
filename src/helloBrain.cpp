// Include C++ packages
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>
// Include my files
#include "includes/files.cpp"
#include "includes/strings.cpp"
// Include the brainflow package
#include "board_shim.h"
#include "data_filter.h"
// Using the standard namespace
using namespace std;

// Establish the setup function
void setup(string location, vector<string> &lexicon, int &board_id, int &port, int &sample_time, int &sample_number);

// Main function
int main(int argc, char *argv[]) {
    // Disable board logging
    BoardShim::disable_board_logger();
    // Create the input parameter structure 
    struct BrainFlowInputParams params;
    // Establish the board ID
    int board_id = 5;
    // Establish the sample time
    int sample_time = 2;
    // Establish the sample number
    int sample_number = 10;
    // Establish the lexicon vector to store prompts
    vector<string> lexicon;
    // Establish the variables from the setup file
    setup("setup.txt", lexicon, board_id, params.ip_port, sample_time, sample_number);
    // Print the initialisation message
    print("Connecting...");
    // Create a board
    BoardShim *board = new BoardShim(board_id, params);
    // Try
    try {
        // Prepare the board
        board->prepare_session();
        // Start the data stream
        board->start_stream();
        // For 10 rounds
        for (int a = 0; a < sample_number; a++) {
            // For each item in the lexicon
            for (string word : lexicon) {
                // Print the word
                print(word);
                // Sleep for the sample time
                sleep(sample_time);
                //
                print(to_string(board->get_board_data_count()));
                // Get the board data and wipe the data from the board
                BrainFlowArray<double, 2> data = board->get_board_data();
                //
                DataFilter::write_file (data,"output/"+word+"-"+to_string(a)+".txt", "w");
                //
                BrainFlowArray<double, 2> restored_data = DataFilter::read_file ("output/"+word+"-"+to_string(a)+".txt");
                // Save the data to a file
                //saveFile("output/"+word+"-"+to_string(a)+".txt", data);
            }
        }
        // End the data stream
        board->stop_stream();
        // Release the board, ending the session
        board->release_session();
    // If there's an error
    } catch (const BrainFlowException &err) {
        // Print an error message
        print("ERROR!");
        // If the board has been prepared
        if (board->is_prepared()) {
            // Release the board, ending the session
            board->release_session();
        }
    }
    // Delete the board object
    delete board;
    // return 0 (success)
    return 0;
}

// The setup function gets details from the setup.txt file
void setup(string location, vector<string> &lexicon, int &board_id, int &port, int &sample_time, int &sample_number) {
    // Get the text file contents
    vector<string> file_contents = readFile(location);
    // Get the lexicon from the file
    lexicon = split(split(file_contents.at(0), '-').at(1), ',');
    // Get the board ID from the file
    board_id = stoi(strip(split(file_contents.at(1), '-').at(1)));
    // Get the port from the file
    port = stoi(strip(split(file_contents.at(2), '-').at(1)));
    // Get the sample_time from the file
    sample_time = stoi(strip(split(file_contents.at(3), '-').at(1)));
    // Get the sample number from the file
    sample_number = stoi(strip(split(file_contents.at(4), '-').at(1)));
}