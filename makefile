CC = gcc
OPTS = -Wall

all: server client

client: client.o commonCore.o
	$(CC) $(CFLAGS) commonCore.o client.o -o client

server: server.o commonCore.o
	$(CC) $(CFLAGS) commonCore.o server.o -o server

clean:
	rm -f *.o *.bak ex


client.o: client.c
	$(CC) $(CFLAGS) -O0 -c client.c

server.o: server.c
	$(CC) $(CFLAGS) -O0 -c server.c

commonCore.o: commonCore.c
	$(CC) $(CFLAGS) -O0 -c commonCore.c