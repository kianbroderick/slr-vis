CC = cc
CFLAGS = $(shell pkg-config --cflags raylib)
LIBS = $(shell pkg-config --libs raylib)

main: main.o stats.o
	$(CC) main.o stats.o $(CFLAGS) $(LIBS) -o main

main.o: main.c stats.h
	$(CC) $(CFLAGS) -c main.c -o main.o

stats.o: stats.c stats.h
	$(CC) -c stats.c $(CFLAGS) -o stats.o


clean:
	rm -f main main.o stats.o
