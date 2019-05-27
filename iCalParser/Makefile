CC = gcc
CPPFLAGS=
CFLAGS = -Wall -std=c11 -g -Iinclude
LDFLAGS= -L./bin -Wl,-rpath,./bin

all: list parser main
list: bin/liblist.so
parser: bin/libcal.so

bin/liblist.so: bin/LinkedListAPI.o
	$(CC) -shared -o bin/liblist.so bin/LinkedListAPI.o

bin/libcal.so: bin/CalendarParser.o 
	$(CC) -shared -o bin/libcal.so bin/LinkedListAPI.o

bin/LinkedListAPI.o: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) $(CFLAGS) -c -fpic src/LinkedListAPI.c -o bin/LinkedListAPI.o

bin/CalendarParser.o: src/CalendarParser.c include/CalendarParser.h include/helper.h include/LinkedListAPI.h
	$(CC) $(CFLAGS) -c src/CalendarParser.c -o bin/CalendarParser.o

bin/helper.o: src/helper.c
	$(CC) $(CFLAGS) -c src/helper.c -o bin/helper.o

bin/test.o: src/test.c 
	$(CC) $(CFLAGS) -c src/test.c -o bin/test.o

main: bin/test.o bin/CalendarParser.o bin/helper.o bin/LinkedListAPI.o
	$(CC) $(CFLAGS) bin/test.o bin/CalendarParser.o bin/helper.o bin/LinkedListAPI.o -o main

clean:
	rm -rf bin/*.o bin/*.so
