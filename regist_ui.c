#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "login_ui.h"
#include "con_server.h"


struct REGIST_MASSAGE
{
    GtkWidget* text_username;
    GtkWidget* text_password;
    GtkWidget* text_repeat_password;
} regist_massage;
GtkWidget* regist_window;


void signup_button_on_click(GtkWidget *widget, struct REGIST_MASSAGE* regist_massage);
void regist_to_login(GtkWidget *widget, gpointer data);
int create_regist_window();


void signup_button_on_click(GtkWidget *widget, struct REGIST_MASSAGE* regist_massage)
{
    const char *username = gtk_entry_get_text(GTK_ENTRY(regist_massage->text_username));
    const char *password = gtk_entry_get_text(GTK_ENTRY(regist_massage->text_password));
    const char *repeat_password = gtk_entry_get_text(GTK_ENTRY(regist_massage->text_repeat_password));

    // local check

    printf("regist username:%s password:%s", username, password);
    
    if(send_signup (username, password) == 0)
    {
        puts(" yes");
    }
    else
    {
        puts(" no");
        //password or username error
    }
}

void regist_to_login(GtkWidget *widget, gpointer data)
{
    gtk_widget_hide(regist_window);
    gtk_widget_show_all(login_window);
}

int create_regist_window()
{
    GtkBuilder *builder = gtk_builder_new();
    if(gtk_builder_add_from_file(builder, "regist.glade", NULL) == 0)
    {
        puts("failed to open regist.glade!");
        return 1;
    }

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));
    GtkWidget *button_signup = GTK_WIDGET(gtk_builder_get_object (builder, "signup"));
    GtkWidget *button_cancel = GTK_WIDGET(gtk_builder_get_object (builder, "cancel"));
    GtkWidget *text_username = GTK_WIDGET(gtk_builder_get_object (builder, "username"));
    GtkWidget *text_password = GTK_WIDGET(gtk_builder_get_object (builder, "password"));
    GtkWidget *text_repeat_password = GTK_WIDGET(gtk_builder_get_object (builder, "repeat_password"));

    regist_massage.text_username = text_username;
    regist_massage.text_password = text_password;
    regist_massage.text_repeat_password = text_repeat_password;

    g_signal_connect (G_OBJECT(button_signup), "clicked", G_CALLBACK (signup_button_on_click), &regist_massage);
    g_signal_connect (G_OBJECT(button_cancel), "clicked", G_CALLBACK (regist_to_login), NULL);
    g_signal_connect (G_OBJECT(window), "delete_event", G_CALLBACK (gtk_main_quit), NULL);

    regist_window =  window;

    return 0;
}