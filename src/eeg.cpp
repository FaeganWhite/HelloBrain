// Include C++ packages
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <vector>
#include "includes/files.cpp"
#include "includes/strings.cpp"
#include "board_shim.h"
#include "data_filter.h"
// Using the standard namespace
using namespace std;


// Establish the baord
BoardShim *board = NULL;

// Connect to the OpenBCI board
bool connect(int port, int frequency) {
    // Try
    try {
        // Disable board logging
        BoardShim::disable_board_logger();
        // Create the input parameter structure 
        struct BrainFlowInputParams params;
        // Set the port number
        params.ip_port = port;
        // Create a board
        board = new BoardShim(5, params);
        // Print an alert
        print("Created board");
        // To prepare a session with the board
        board->prepare_session();
        // Print an alert
        print("Session prepared");
        // return the board
        return true;
    // If there is an error with the connection
    } catch (const BrainFlowException &err) {
        // Set the board as NULL
        board = NULL;
        // Return there was a failure
        return false;
    }
}

// Disconnect from the OpenBCI board
bool disconnect() {
    // If the board is prepared
    if (board->is_prepared()) {
        // Try
        try {
            // Print an alert
            print("Disconnecting");
            // Release the board
            board->release_session();
            // Set the board to NULL
            board = NULL;
            // Return the disconnectioon
            return false;
        // If there is a error
        } catch (const BrainFlowException &err) {
            // Return the board as still connected
            return true;
        }
    }
    // Return that the board isn't connected
    return false;
}

// Return of the board is connected
bool check_connected() {
    // If the board is NULL
    if (board == NULL) {
        // Return false
        return false;
    }
    // If the board is connected
    if (board->is_prepared()) {
        // Return true
        return true;
    }
    // Otherwise, return false
    return false;
}