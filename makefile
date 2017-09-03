src = main.c login_ui.c regist_ui.c con_server.c  chat_ui.c error_dialog_ui.c com_with_server.c
uTalk: $(src)
	gcc $(src) -o uTalk -g -lpthread --std=c99 `pkg-config --cflags --libs gtk+-3.0`