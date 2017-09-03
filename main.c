#include <stdio.h>
#include "login_ui.h"
#include "regist_ui.h"
#include "con_server.h"


int main (int argc, char** argv)
{

    if(online() != 0)
    {
        puts("failed to connect server!");
        return 0;
    }

    gtk_init(&argc, &argv);

    if(create_login_window() != 0)
    {
        puts("failed to create login window!");
    }
    if(create_regist_window() != 0)
    {
        puts("failed to create regist window!");
    }
    
    gtk_widget_show_all (login_window);

    gtk_main();

    return 0;
}