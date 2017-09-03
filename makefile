src = main_dada.c  chat_ui.c error_dialog_ui.c fake_com_with_server.c
uTalk: $(src)
	gcc $(src) -o uTalk -g -pthread `pkg-config --cflags --libs gtk+-3.0`