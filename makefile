src = main_dada.c chat_ui.c friendbox_struct.c virtual_msg_server.c msg_queue.c
uTalk: $(src)
	gcc $(src) -o uTalk -g -pthread `pkg-config --cflags --libs gtk+-3.0`