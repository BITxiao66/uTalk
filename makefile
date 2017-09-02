src = main2.c  chat_ui.c error_dialog_ui.c com_with_server.c
uTalk: $(src)
	gcc $(src) -o uTalk -g -pthread `pkg-config --cflags --libs gtk+-3.0`