#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "regist_ui.h"
#include "con_server.h"
#include "chat_ui.h"

struct LOGIN_MASSAGE
{
    GtkWidget* text_username;
    GtkWidget* text_password;
} login_massage;
GtkWidget* login_window;


void signin_button_on_click(GtkWidget *widget, struct LOGIN_MASSAGE* login_massage);
void login_to_regist(GtkWidget *widget, gpointer data);
int create_login_window();


void signin_button_on_click(GtkWidget *widget, struct LOGIN_MASSAGE* login_massage)
{
    const char *username = gtk_entry_get_text(GTK_ENTRY(login_massage->text_username));
    const char *password = gtk_entry_get_text(GTK_ENTRY(login_massage->text_password));

    if(strlen(username) == 0)
    {
        puts("username can't be empty!");
        return;
    }

    if(strlen(password) == 0)
    {
        puts("password can't be empty!");
        return;
    }

    printf("login username:%s password:%s", username, password);

    if(send_login(username, password) == 0)
    {
        puts(" yes");
        if(chat_ui (username) != 0)
        {
            puts("failed to open main_ui!");
        }
    }
    else
    {
        puts(" no");
        //password or username error
    }
}

void login_to_regist(GtkWidget *widget, gpointer data)
{
    gtk_widget_hide(login_window);
    gtk_widget_show_all(regist_window);
}

int create_login_window()
{

    GtkBuilder *builder = gtk_builder_new();
    if(gtk_builder_add_from_file(builder, "login.glade", NULL) == 0)
    {
        puts("failed to open login.glade!");
        return 1;
    }

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));
    GtkWidget *button_signin = GTK_WIDGET(gtk_builder_get_object (builder, "signin"));
    GtkWidget *button_regist = GTK_WIDGET(gtk_builder_get_object (builder, "regist"));
    GtkWidget *text_username = GTK_WIDGET(gtk_builder_get_object (builder, "username"));
    GtkWidget *text_password = GTK_WIDGET(gtk_builder_get_object (builder, "password"));

    login_massage.text_username = text_username;
    login_massage.text_password = text_password;

    g_signal_connect (G_OBJECT(button_signin), "clicked", G_CALLBACK (signin_button_on_click), &login_massage);
    g_signal_connect (G_OBJECT(button_regist), "clicked", G_CALLBACK (login_to_regist), NULL);
    g_signal_connect (G_OBJECT(window), "delete_event", G_CALLBACK (gtk_main_quit), NULL);

    login_window =  window;

    return 0;
}