CC = gcc
CFLAGS = -Wall -Iinclude -std=c23 -fdiagnostics-color=always -g
CLIB = -Llib -lSDL3 -lchelp

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, bin/%.o, $(SRC))

all: always $(OBJ) finish

always:
	mkdir -p bin

bin/%.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

finish:
	gcc $(wildcard bin/*.o) -o bin/pathtracer.exe $(CLIB)
# 	rm bin/*.o