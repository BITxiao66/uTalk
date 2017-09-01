uTalk: uTalk.c
	gcc uTalk.c -o uTalk -g `pkg-config --cflags --libs gtk+-3.0`