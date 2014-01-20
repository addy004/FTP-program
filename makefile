CC=gcc
CFLAGS=-I.
DEPS=ftphelper.h
CLIOBJ=client.o
SERVOBJ=server.o

all: client server

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

client: $(CLIOBJ)
	$(CC) -o $@ $^ $(CFLAGS)

server: $(SERVOBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *o client server