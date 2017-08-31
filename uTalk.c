#include <gtk/gtk.h>

GtkApplication *app;//uTalk application

/*
 * Part : All functions' declaretion.
 * Author : xinchengxin
 * Date : 2017.08.31
 * Time : 15:38:06
 */
static void user_signin(GtkWidget* widget, gpointer data);
static void user_signup(GtkWidget* widget, gpointer data);
static void to_signin(GtkWidget *window);
static void to_signup(GtkWidget *window);
static void create_signup_window();
static void create_signin_window();

/*
 * Purpose : After user clicked the signin button in signin window, signin.
 * Prameter : Function is not completed.
 * Return : void
 * Side effect : Function is not completed.
 * Author : xinchengxin
 * Date : 2017.08.31
 * Time : 15:44:06
 */
static void user_signin(GtkWidget* widget, gpointer data)
{
    //Function is not completed.
}

/*
 * Purpose : After user clicked the signup button in signup window, signup.
 * Prameter : Function is not completed.
 * Return : void
 * Side effect : Function is not completed.
 * Author : xinchengxin
 * Date : 2017.08.31
 * Time : 15:44:59
 */
static void user_signup(GtkWidget* widget, gpointer data)
{
    //Function is not completed.
}

/*
 * Purpose : After user clicked the signup button in signup window, destroy the signup window, submit the data to server and create a signin window.
 * Prameter : The signup window.
 * Return : void
 * Side effect : None.
 * Author : xinchengxin
 * Date : 2017.08.31
 * Time : 15:53:49
 */
static void to_signin(GtkWidget *window)
{
    gtk_widget_destroy(window);
    create_signin_window();
}

/*
 * Purpose : After user clicked the signup button in signin window, destroy the signin window and create a signup window.
 * Prameter : The signin window.
 * Return : void
 * Side effect : None.
 * Author : xinchengxin
 * Date : 2017.08.31
 * Time : 15:51:59
 */
static void to_signup (GtkWidget *window)
{
    gtk_widget_destroy(window);
    create_signup_window();
}

/*
 * Purpose : Create a signin window.
 * Prameter : None.
 * Return : void
 * Side effect : None.
 * Author : xinchengxin
 * Date : 2017.08.31
 * Time : 15:54:22
 */
static void create_signin_window()
{
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "signin.glade", NULL);

    GtkWidget *window = gtk_application_window_new (app);
    window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));

    GtkWidget *button_signin = GTK_WIDGET(gtk_builder_get_object (builder, "signin"));
    GtkWidget *button_signup = GTK_WIDGET(gtk_builder_get_object (builder, "signup"));
    
    g_signal_connect (button_signin, "clicked", G_CALLBACK (user_signin), NULL);
    g_signal_connect_swapped (button_signup, "clicked", G_CALLBACK (to_signup), window);

    gtk_widget_show_all(window);
}

/*
 * Purpose : Create a signup window.
 * Prameter : None.
 * Return : void
 * Side effect : None.
 * Author : xinchengxin
 * Date : 2017.08.31
 * Time : 15:54:46
 */
static void create_signup_window()
{
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "signup.glade", NULL);

    GtkWidget *window = gtk_application_window_new (app);
    window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));

    GtkWidget *button_signup = GTK_WIDGET(gtk_builder_get_object (builder, "signin"));
    GtkWidget *button_cancel = GTK_WIDGET(gtk_builder_get_object (builder, "cancel"));
    
    g_signal_connect (button_signup, "clicked", G_CALLBACK (user_signup), NULL);
    g_signal_connect_swapped (button_cancel, "clicked", G_CALLBACK (to_signin), window);

    gtk_widget_show_all(window);
}


/*
 * Purpose : Start the uTalk Application.
 * Prameter : the uTalk Application, user data
 * Return : void
 * Side effect : None.
 * Author : xinchengxin
 * Date : 2017.08.31
 * Time : 15:56:00
 */
static void start (GtkApplication* app, gpointer user_data)
{
    create_signin_window();
}

/*
 * Purpose : the main funcation, creating this application.
 * Prameter : int argc, char** argv
 * Return : int
 * Side effect : None.
 * Author : xinchengxin
 * Date : 2017.08.31
 * Time : 15:56:51
 */
int main (int argc, char** argv)
{

    app = gtk_application_new("edu.bit.utalk", G_APPLICATION_FLAGS_NONE);

    g_signal_connect (app, "activate", G_CALLBACK (start), NULL);

    int status = g_application_run (G_APPLICATION (app), argc, argv);

    g_object_unref (app);
 
    return status;
}