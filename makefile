src = main.c chat.c friendbox_struct.c msg_queue.c client.c
uTalk: $(src)
	gcc $(src) -o uTalk -g -pthread `pkg-config --cflags --libs gtk+-3.0`