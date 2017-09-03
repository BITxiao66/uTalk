#include <gtk/gtk.h>
#include "error_dialog_ui.h"

void error_dialog (GtkWidget *window, gchar *error_msg){
    GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", error_msg);
    gtk_window_set_title(GTK_WINDOW(dialog), "Error");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}