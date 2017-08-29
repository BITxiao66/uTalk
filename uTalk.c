#include <gtk/gtk.h>

void signup(GtkWidget *btn, gpointer data){
    GtkBuilder *builder = gtk_builder_new_from_file ("signup.glade");
}

int main (int argc, char *argv[]){
    gtk_init (&argc, &argv);
    GtkBuilder *builder = gtk_builder_new_from_file ("signin.glade");
    g_signal_connect (gtk_builder_get_object(builder, "window"), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect (gtk_builder_get_object(builder, "signin"), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect (gtk_builder_get_object(builder, "signup"), "clicked", G_CALLBACK(signup), NULL);
    gtk_main();
    return 0;
}