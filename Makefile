CC = gcc
CFLAGS = -Wall -Iinclude -std=c23 -fdiagnostics-color=always -g
CLIB = -Llib -lSDL3

all: always main

always:
	mkdir -p bin

main:
	$(CC) src/pathtracer.c -o bin/pathtracer.exe $(CFLAGS) $(CLIB)