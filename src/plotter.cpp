#include <iostream>
#include <fstream>
#include <vector>
#include "pbplot/pbPlots.cpp"
#include "pbplot/supportLib.cpp"
#include "board_shim.h"
using namespace std;

// Plot data as a graph
void plot(int height, int width, vector<double> x_data, vector<double> y_data, string file_name){
    // Create a bool to store whether the graph had been creates
    bool created;
    // Create a string referenc to hold any errors
    StringReference *error_message = new StringReference();
    // Create an image reference to hold the graph
	RGBABitmapImageReference *imageReference = CreateRGBABitmapImageReference();
    // Draw the scatter plot
	created = DrawScatterPlot(imageReference, height, width, &x_data, &y_data, error_message);
    // If the plot was created successfully
    if (created) {
        // Convert the graph to a PNG
        vector<double> *png_data = ConvertToPNG(imageReference->image);
        // Save the PNG as the file
        WriteToFile(png_data, file_name+".png");
        // Delete the image to free memory
        DeleteImage(imageReference->image);
    }
}

// Save an EEG file as a set of plots
void plot_file(BrainFlowArray<double, 2> data, int data_point_count, int height, int width, string file_name, int accuracy) {
    // Try
    try {
        // Get the data from the file
        //vector<string> data = read_file(file_name);
        // Establish the x data
        vector<double> x_data;
        // Establish the y data
        //vector<double> y_data[8];
        // Establish the counter
        //int counter = 0;
        // Establish a file delimeter
        //string delimeter = "	";
        // For every line in the file
        //for (int a = 0; a < data.size(); a += accuracy) {
            // Get the line of data
        //    string line = data.at(a);
            // Split the line into data points
        //    vector<string> split_line = split(line, delimeter);
            // For each electrode
        //    for (int i = 0; i < 8; i++) {
                // Add the data to the y_data vector
        //        y_data[i].push_back(stod(split_line.at(i+1)));
        //    }
            // Add the time value to the counter
        //    x_data.push_back(counter);
            // Increment the counter
        //    counter++;
        //}
        
        for (int i = 0; i < data_point_count; i++) {
            //
            x_data.push_back(i);
        }
        // For each electrode
        for (int a = 0; a < 8; a++) {
            //
            vector<double> y_data;
            //
            for (int i = 0; i < data_point_count; i++) {
                //
                y_data.push_back(data.get_address(a+1)[i]);
            }
            // Create a plot
            plot(height, width, x_data, y_data, "plots/image_"+to_string(a+1));
        }
    // If there's an error
    } catch(...) {
        // Print an error message
        print("GRAPHING ERROR");
    }
}