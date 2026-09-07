CC = cc
CFLAGS = $(shell pkg-config --cflags raylib)
LIBS = $(shell pkg-config --libs raylib)

main: main.o slr.o time.o plotting.o
	$(CC) main.o slr.o time.o plotting.o $(CFLAGS) $(LIBS) -o main

main.o: main.c slr.h
	$(CC) $(CFLAGS) -c main.c -o main.o

slr.o: slr.c slr.h
	$(CC) -c slr.c $(CFLAGS) -o slr.o

plotting.o: plotting.c plotting.h
	$(CC) -c plotting.c $(CFLAGS) -o plotting.o

time.o: time.c time.h
	$(CC) -c time.c $(CFLAGS) -o time.o



clean:
	rm -f main main.o slr.o time.o plotting.o
