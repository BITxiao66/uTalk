#include <gtk/gtk.h>
#include "fake_com_with_server.h"

gboolean send_msg_to_server(const gchar *chat_friend_name, const gchar *msg){
    return TRUE;
}

gboolean build_chat_with_server(const gchar *chat_friend_name){
    return TRUE;
}

gboolean request_friends_list_from_server(gchar *username, gint *friends_num, gchar *friends_list[], gchar *msg_list[]){
    *friends_num = 2;
    friends_list[0] = "ABC";
    msg_list[0] = "last msg";
    friends_list[1] = "xyz";
    msg_list[1] = "last msg";
    return TRUE;
}
