CC=gcc
CFLAGS=-I. -Wall
HELPERS=requests.c helpers.c buffer.c parson.c functions.c

client: client.c requests.c helpers.c buffer.c
	$(CC) -o client client.c $(HELPERS) $(CFLAGS)

run: client
	./client

clean:
	rm -f *.o client
