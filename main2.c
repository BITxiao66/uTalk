#include <gtk/gtk.h>
#include "chat_ui.h"
#include "error_dialog_ui.h"

int main (){
    gboolean load_chat_ui_succeed = chat_ui("DaDaMr_X");
    // if (load_chat_ui_succeed == FALSE){
    //     error_dialog (window, "Load chat UI faild!");
    // }
    return 0;
}