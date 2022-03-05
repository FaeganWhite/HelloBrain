#include <gtk/gtk.h>

// Get an int from a UI field
int get_entry_int(GObject *entry) {
    // Return an int from the UI field
    return atoi(gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(entry))));
}

// Get a string from a UI field
string get_entry_string(GObject *entry) {
    // Return a string from the UI field
    return gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(entry)));
}