CC=clang
CFLAGS=-O2 -std=gnu23 -Wall -Werror -Wno-pointer-sign -Werror=vla
LDFLAGS=-ldl
LD=clang

all: compile

compile:
	@$(CC) $(CFLAGS) main.c -c -o main.o
	@$(CC) $(CFLAGS) linear.c -c -o linear.o
	@$(CC) $(CFLAGS) mt.c -c -o mt.o
	@$(CC) $(CFLAGS) mtwister.c -c -o mtwister.o
	@$(LD) main.o linear.o mt.o mtwister.o -o printrandom $(LDFLAGS) -rdynamic

clean:
	@rm linear.o main.o mtwister.o mt.o

