#include <gtk/gtk.h>

// Activate the application
static void activate (GtkApplication *application) {
    // Create a new window
    GtkWidget *window = gtk_application_window_new(application);
    // Set the title of the window
    gtk_window_set_title(GTK_WINDOW(window), "Hello Brain");
    // Set the default size of the window
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
    // Show the window
    gtk_widget_show(window);
    // Create a new button
    //GtkWidget *button = gtk_button_new_with_label("Hello, World!");
    // When the button is clicked, close the window passed as an argument
    //g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_close), window);
    // Add the button to the window
    //gtk_window_set_child(GTK_WINDOW(window), button);
    // Show the window
    //gtk_window_present(GTK_WINDOW(window));
}

// Main function - creates GTK application and runs it
int main(int argc, char **argv) {
    // Create a new application with an identifier
    GtkApplication *application = gtk_application_new("com.github.FaeganWhite.HelloBrain", G_APPLICATION_FLAGS_NONE);
    //
    g_signal_connect(application, "activate", G_CALLBACK(activate), NULL);
    // Run the application and get the status
    int status = g_application_run(G_APPLICATION(application), argc, argv);
    // Decrease the reference count of the application
    g_object_unref(application);
    // Return the status of the program
    return status;
    //
    //return g_application_run(G_APPLICATION(application), argc, argv);
}