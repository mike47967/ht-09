CC=gcc
CFLAGS=-O2 -std=gnu2x -Wall -Werror -Wno-pointer-sign -Werror=vla
LD=gcc
LDFLAGS=-ldl

all: compile

compile:
	@$(CC) $(CFLAGS) main.c -c -o main.o
	@$(CC) $(CFLAGS) linear.c -c -o linear.o
	@$(CC) $(CFLAGS) mt.c -c -o mt.o
	@$(LD) main.o linear.o mt.o -o printrandom $(LDFLAGS) -rdynamic
clean:
	@rm -f linear.o mt.o main.o printrandom
