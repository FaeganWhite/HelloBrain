#include <gtk/gtk.h>
#include <string>
#include <gdk/gdk.h>
#include <math.h>
#include "eeg.cpp"
#include "plotter.cpp"
#include "includes/ui.cpp"
#include "board_shim.h"
#include <stdlib.h>
using namespace std;

// Create a connection structure
struct Connection{
    // Establish the port
    int port;
    // Establish the reading frequency
    int frequency;
};

// Establish the structures
Connection current_connection;
// Establish the GUI elements
GObject *port_field;
GObject *frequency_field;
GObject *sample_time_field;
GObject *sample_number_field;
GObject *lexicon_field;
GObject *session_name_field;
GObject *remove_end_check;
GObject *remove_outlier_check;
GObject *feedback_check;
GObject *save_check;
GObject *connect_button;
GObject *start_button;
GObject *pause_button;
GObject *stop_button;
GObject *connect_label;
GObject *prompt_label;
GObject *remaining_samples_label;
GObject *image_1;
GObject *image_2;
GObject *image_3;
GObject *image_4;
GObject *image_5;
GObject *image_6;
GObject *image_7;
GObject *image_8;
// Establish the reading variables
gboolean currently_reading = FALSE;
gboolean currently_connected = FALSE;
string last_button = "start";
int current_read_number = 0;
vector<string> lexicon;
bool currently_recording;
const char * current_prompt;
int sample_time;

// Update the GUI images
gboolean update_images(gpointer data) {
    // Update the images from the image files
    gtk_image_set_from_file(GTK_IMAGE(image_1), "plots/image_1.png");
    gtk_image_set_from_file(GTK_IMAGE(image_2), "plots/image_2.png");
    gtk_image_set_from_file(GTK_IMAGE(image_3), "plots/image_3.png");
    gtk_image_set_from_file(GTK_IMAGE(image_4), "plots/image_4.png");
    gtk_image_set_from_file(GTK_IMAGE(image_5), "plots/image_5.png");
    gtk_image_set_from_file(GTK_IMAGE(image_6), "plots/image_6.png");
    gtk_image_set_from_file(GTK_IMAGE(image_7), "plots/image_7.png");
    gtk_image_set_from_file(GTK_IMAGE(image_8), "plots/image_8.png");
    // Only run function once
    return FALSE;
}

// Asyncronously check if the board is connected
gboolean async_check_connected(gpointer data) {
    // If the board isn't connected
    if (!check_connected()) {
        // Set the connected boolean
        currently_connected = FALSE;
    }
    // Return the connected boolean to control the loop
    return currently_connected;
}

// Update the training prompt label
gboolean async_update_prompt(gpointer data) {
    // Update the label
    gtk_label_set_label(GTK_LABEL(prompt_label), current_prompt);
    // Return false to not repeat
    return FALSE;
}

// Clear the training prompt
gboolean async_clear_prompt(gpointer data) {
    // Update the label
    gtk_label_set_label(GTK_LABEL(prompt_label), "");
    // Return false to not repeat
    return FALSE;
}

// Asyncronously get data from the board
gboolean async_read_loop(gpointer data) {
    // If the start button was pressed
    if (last_button == "start") {
        // Update the prompt label
        gtk_label_set_label(GTK_LABEL(prompt_label), "");
    }
    // Calculate the position in the lexicon
    int lexicon_position = (lexicon.size()-((current_read_number+(lexicon.size()-1))%lexicon.size()))-1;
    // Get the file name
    string file_name = "output/"+get_entry_string(session_name_field)+"/"+lexicon.at((lexicon_position+lexicon.size()-1)%lexicon.size())+"_"+split(to_string(floor(((current_read_number)/lexicon.size())+1)), ".").at(0)+".txt";
    // If the board is connected and data is being read
    if (check_connected() && currently_reading == TRUE) {
        // Clear the prompt
        gtk_label_set_label(GTK_LABEL(prompt_label), "");
        // Get the next prompt from the lexicon
        current_prompt = lexicon.at(lexicon_position).c_str();
        // Asynchronously update the prompt from the lexicon
        g_timeout_add(1000, async_update_prompt, NULL);
        // Asynchronously clear the prompt
        g_timeout_add(sample_time+1000, async_clear_prompt, NULL);
        // Get the position through the sample collection
        string sample_position = to_string(((current_read_number-1)%lexicon.size())+1)+" / "+split(to_string(floor(((current_read_number-1)/lexicon.size())+1)), ".").at(0);
        // Update the label
        gtk_label_set_label(GTK_LABEL(remaining_samples_label), sample_position.c_str());
        // Get the board data, wipe the data from the board and get the number of data points
        BrainFlowArray<double, 2> data = board->get_board_data();
        // If the program isn't currently recording
        if (!currently_recording) {
            // Start recording
            currently_recording = true;
        // Otherwise, if recording has started
        } else {
            //
            if (gtk_check_button_get_active(GTK_CHECK_BUTTON(save_check)) == TRUE) {
                // Save the data to a file
                DataFilter::write_file(data, file_name, "w");
            }
            // If the feedback check button is checked
            if (gtk_check_button_get_active(GTK_CHECK_BUTTON(feedback_check)) == TRUE) {
                // Create plots from the EEG data
                plot_file(data, data.get_size(1), 100, 100, file_name, 5);
                // Update all the images
                g_timeout_add(100, update_images, NULL);
            }
            // Flush the boards internal buffer
            data = board->get_board_data();
        }
        // If that was the last reading
        if (current_read_number <= 0) {
            // If the start button was pressed (stream hasn't been stopped)
            if (last_button == "start") {
                // Update the prompt label
                gtk_label_set_label(GTK_LABEL(prompt_label), "");
            }
            // Stop the reading
            currently_reading = FALSE;
        }
        // Decrement the reading number
        current_read_number--;
    // Otherwise, if disconnected
    } else {
        // Stop the reading
        currently_reading = FALSE;
        // If the start button was pressed (stream hasn't been stopped)
        if (last_button == "start") {
            // Update the prompt label
            gtk_label_set_label(GTK_LABEL(prompt_label), "Please connect board");
        }
        // If the board is connected
        if (check_connected()) {
            // End the data stream
            board->stop_stream();
        }
    }
    // Return whether to take the next reading
    return currently_reading;
}

// Asyncronously connect to the BCI device
gboolean async_toggle_connect(gpointer data) {
    // Print an alert
    print("Toggling connection");
    // Connect to the board
    bool result;
    // If the board exists
    if (board != NULL) {
        // If the board isn't prepared
        if (!board->is_prepared()) {
            // Get the port number from the UI
            int port = get_entry_int(port_field);
            // Get the frequency from the UI
            int frequency = get_entry_int(frequency_field);
            // Connect to the board and get the result
            result = connect(port, frequency);
        // Otherwise
        } else {
            // Disconnect from the board
            result = disconnect();
        }
    // If the board doesn't exist
    } else {
        // Connect to the  board
        result = connect(8000, 1000);
    }
    // If the board is connected
    if (result == true) {
        // Update the button label
        gtk_button_set_label(GTK_BUTTON(connect_button), "Disconnect");
        // Update the label
        gtk_label_set_label(GTK_LABEL(connect_label), "Connected");
        // Set the device as connected
        currently_connected = TRUE;
        // Start the loop to check the connection
        g_timeout_add(2000, async_check_connected, NULL);
        // Print an alert
        print("Connected");
    // otherwise
    } else {
        // Update the button label
        gtk_button_set_label(GTK_BUTTON(connect_button), "Connect");
        // Update the label
        gtk_label_set_label(GTK_LABEL(connect_label), "Disconnected");
        // Print an alert
        print("Disconnected");
    }
    // Return false to only run function once
    return FALSE;
}

// Establish a connection to the OpenBCI device
void connect_to_device(GtkWidget *widget, gpointer *button) {
    // Set the button label to connecting
    gtk_button_set_label(GTK_BUTTON(button), "Connecting...");
    // Asyncronously connect to the board with the connection object
    g_timeout_add(100, async_toggle_connect, NULL);
}

// Start the recording
void start_recording(GtkWidget *widget, gpointer *button) {
    // Make a folder for the session data
    make_folder(get_entry_string(session_name_field));
    // Update the prompt label
    gtk_label_set_label(GTK_LABEL(prompt_label), "Initialising...");
    // Update the last button pressed
    last_button = "start";
    // Establish the delimeter
    string delimiter = ",";
    // Get the lexicon from the lexicon field
    lexicon = strip_vector(split(get_entry_string(lexicon_field), delimiter));
    // If this is the start of the session
    if (current_read_number == 0) {
        // Get the number of readings from the UI and the lexicon size
        current_read_number = get_entry_int(sample_number_field)*lexicon.size();
    }
    // Get the sample time from the UI
    sample_time = (get_entry_int(sample_time_field))*1000;
    // If the board is connected
    if (check_connected()) {
        // Start the recording stream
        board->start_stream();
    } else {
        // Prepare the session with the board
        board->prepare_session();
        // Start the recording stream
        board->start_stream();
    }
    // Set the recording as false
    currently_recording = false;
    // Start the reading loop
    g_timeout_add(sample_time+2000, async_read_loop, NULL);
    // Set the loop to start
    currently_reading = TRUE;
}

// Pause the recording
void pause_recording(GtkWidget *widget, gpointer *button) {
    // Update the last button pressed
    last_button = "pause";
    // Set the reading loop to stop
    currently_reading = FALSE;
    // Stop the recording
    currently_recording = false;
}

// Stop the recording
void stop_recording(GtkWidget *widget, gpointer *button) {
    // Update the last button pressed
    last_button = "stop";
    // Set the reading loop to stop
    currently_reading = FALSE;
    // reset the current read number
    current_read_number = 0;
    // Update the prompt label
    gtk_label_set_label(GTK_LABEL(prompt_label), "");
    // Update the label
    gtk_label_set_label(GTK_LABEL(remaining_samples_label), "...");
    // Stop the recording
    currently_recording = false;
}