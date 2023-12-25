.PHONY: all clean install uninstall
PREFIX?=/usr/local


all:
	-mkdir bin
	$(CC) src/*.c -o bin/disabld