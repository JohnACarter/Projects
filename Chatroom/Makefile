CC = gcc 
TARGET = shell server
OBJECT = shell.o util.o server.o

all: server shell

server: server.o util.o
	$(CC) -g -o server server.o util.o

server.o: server.c
	$(CC) -g -c server.c

shell: shell.o util.o
	$(CC) -g -o shell shell.o util.o

shell.o: shell.c
	$(CC) -g -c shell.c

util.o: util.c util.h
	$(CC) -g -c util.c

clean:
	rm -f $(OBJECT) $(TARGET)
