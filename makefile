uTalk: main.c main.h
	gcc main.c -o uTalk `pkg-config --cflags --libs gtk+-3.0`