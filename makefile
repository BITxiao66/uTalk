uTalk: main2.c  chat_ui.c error_dialog_ui.c com_with_server.c
	gcc main2.c  chat_ui.c error_dialog_ui.c com_with_server.c -o uTalk -g `pkg-config --cflags --libs gtk+-3.0`