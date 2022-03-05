#include <gtk/gtk.h>
#include "control.cpp"

// Establish the program's buttons
static void establish_buttons(GtkBuilder *builder) {
    // Establish the start button
    connect_button = gtk_builder_get_object(builder, "connect_button");
    // Add a function to the connect_button
    g_signal_connect(connect_button, "clicked", G_CALLBACK(connect_to_device), connect_button);
    // Establish the start button
    start_button = gtk_builder_get_object(builder, "start_button");
    // Add a function to the start button
    g_signal_connect(start_button, "clicked", G_CALLBACK(start_recording), start_button);
    // Establish the pause button
    pause_button = gtk_builder_get_object(builder, "pause_button");
    // Add a function to the pause button
    g_signal_connect(pause_button, "clicked", G_CALLBACK(pause_recording), pause_button);
    // Establish the stop button
    stop_button = gtk_builder_get_object(builder, "stop_button");
    // Add a function to the stop button
    g_signal_connect(stop_button, "clicked", G_CALLBACK(stop_recording), stop_button);
}

// Establish the default values of the program
static void establish_defaults(GtkBuilder *builder) {
    // Establish an entry object
    port_field = gtk_builder_get_object(builder, "port_field");
    // Set the default port
    gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(port_field)), "8000", 4);
    // Establish an entry object
    frequency_field = gtk_builder_get_object(builder, "frequency_field");
    // Set the default frequency
    gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(frequency_field)), "1000", 4);
    // Establish an entry object
    sample_time_field = gtk_builder_get_object(builder, "sample_time_field");
    // Set the default sample time
    gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(sample_time_field)), "1", 1);
    // Establish an entry object
    sample_number_field = gtk_builder_get_object(builder, "sample_number_field");
    // Set the default number of entries
    gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(sample_number_field)), "100", 3);
    // Establish an entry object
    lexicon_field = gtk_builder_get_object(builder, "lexicon_field");
    // Set the default lexicon
    gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(lexicon_field)), "Yes, No", 7);
    // Establish an entry object
    session_name_field = gtk_builder_get_object(builder, "session_name_field");
    // Set the default session name
    gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(session_name_field)), "Session_1", 9);
    // Establish an entry object
    remove_end_check = gtk_builder_get_object(builder, "remove_end_check");
    // Set the default remove end setting
    gtk_check_button_set_active(GTK_CHECK_BUTTON(remove_end_check), true);
    // Establish an entry object
    remove_outlier_check = gtk_builder_get_object(builder, "remove_outlier_check");
    // Set the default remove outlier setting
    gtk_check_button_set_active(GTK_CHECK_BUTTON(remove_outlier_check), true);
    // Establish the feedback checkbox
    feedback_check = gtk_builder_get_object(builder, "feedback_check");
    // Establish the save file checkbox
    save_check = gtk_builder_get_object(builder, "save_check");
    // Set the default feedback setting
    gtk_check_button_set_active(GTK_CHECK_BUTTON(feedback_check), true);
    // Establish the connection indicator label
    connect_label = gtk_builder_get_object(builder, "connect_label");
    // Establish the prompt label
    prompt_label = gtk_builder_get_object(builder, "prompt_label");
    // Establish the reamining samples label
    remaining_samples_label = gtk_builder_get_object(builder, "remaining_samples_label");
    // Establish the images
    image_1 = gtk_builder_get_object(builder, "image_1");
    image_2 = gtk_builder_get_object(builder, "image_2");
    image_3 = gtk_builder_get_object(builder, "image_3");
    image_4 = gtk_builder_get_object(builder, "image_4");
    image_5 = gtk_builder_get_object(builder, "image_5");
    image_6 = gtk_builder_get_object(builder, "image_6");
    image_7 = gtk_builder_get_object(builder, "image_7");
    image_8 = gtk_builder_get_object(builder, "image_8");
    // Establish the feedback check
    feedback_check = gtk_builder_get_object(builder, "feedback_check");
}

// Setup the CSS for the application
static void establish_style(GtkBuilder *builder) {
    // Get the propt box object from the UI
    GObject *box = gtk_builder_get_object(builder, "prompt_box");
    // Create a CSS provider object
    GtkCssProvider *provider = gtk_css_provider_new();
    // Load the CSS from the stylesheet
    gtk_css_provider_load_from_path(provider, "layout/style.css");
    // Create a context object from the prompt box
    GtkStyleContext *context = gtk_widget_get_style_context(GTK_WIDGET(box));
    // Add the CSS provider to the prompt box context
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    // Get the propt box object from the UI
    GObject *label = gtk_builder_get_object(builder, "prompt_label");
    // Create a context object from the prompt label
    context = gtk_widget_get_style_context(GTK_WIDGET(label));
    // Add the CSS provider to the prompt label context
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

// Activate the application
static void activate(GtkApplication *application) {
    // Create a builder object
    GtkBuilder *builder = gtk_builder_new();
    // Get a layout from the layout file
    gtk_builder_add_from_file(builder, "layout/hellobrain.ui", NULL);
    // Create a new window from the builder
    GObject *window = gtk_builder_get_object(builder, "window");
    // Set the title of the window
    gtk_window_set_title(GTK_WINDOW(window), "Hello Brain");
    // Set the application to the window
    gtk_window_set_application(GTK_WINDOW(window), application);
    // Establish the buttons of the UI
    establish_buttons(builder);
    // Establish the buttons of the UI
    establish_defaults(builder);
    // Establish the style of the app
    establish_style(builder);
    // Show the window
    gtk_widget_show(GTK_WIDGET(window));
    // Unreference the builder object
    g_object_unref(builder);
}

// Main function - creates GTK application and runs it
int main(int argc, char **argv) {
    // Create a new application with an identifier
    GtkApplication *application = gtk_application_new("com.github.FaeganWhite.HelloBrain", G_APPLICATION_FLAGS_NONE);
    // Activate the application
    g_signal_connect(application, "activate", G_CALLBACK(activate), NULL);
    // Run the application and get the status
    int status = g_application_run(G_APPLICATION(application), argc, argv);
    // Decrease the reference count of the application
    g_object_unref(application);
    // Return the status of the program
    return status;
}