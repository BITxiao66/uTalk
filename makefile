uTalk : main.c login_ui.c regist_ui.c con_server.c login_ui.h regist_ui.h con_server.h
	gcc main.c login_ui.c regist_ui.c con_server.c -o uTalk -g -lpthread --std=c99 `pkg-config gtk+-3.0 --cflags --libs gtk+-3.0`
