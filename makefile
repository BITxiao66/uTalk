src = main.c chat.c friendbox_struct.c msg_queue.c client_test.c
uTalk: $(src)
	gcc $(src) -o uTalk -g -lpthread --std=c99 `pkg-config --cflags --libs gtk+-3.0`