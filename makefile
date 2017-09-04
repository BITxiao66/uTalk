src = main_dada.c chat_ui.c friendbox_struct.c msg_queue.c virtual_msg_server.c
uTalk: $(src)
	gcc $(src) -o uTalk -g -lpthread --std=c99 `pkg-config --cflags --libs gtk+-3.0`