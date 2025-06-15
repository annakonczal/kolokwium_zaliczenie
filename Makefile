CC=gcc
CFLAGS=-Wall -g

all: main

server.o: server.c server.h client.h
	$(CC) $(CFLAGS) -c server.c

client.o: client.c client.h
	$(CC) $(CFLAGS) -c client.c

main.o: main.c server.h
	$(CC) $(CFLAGS) -c main.c

main: main.o server.o client.o
	$(CC) $(CFLAGS) -o main main.o server.o client.o

clean:
	rm -f *.o main
