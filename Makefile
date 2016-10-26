BIN=testmain
SOURCES=buffer.c error.c testmain.c index.c
OBJS=buffer.o error.o testmain.o index.o
HEADERS=buffer.h error.h index.h
CC=gcc
FLAGS = -c -Wall
CHECK = -lcheck -lrt

$(BIN): $(OBJS)
	$(CC) -g -o $@ $(OBJS) $(CHECK)

testmain.o: testmain.c
	$(CC) $(FLAGS) $?

buffer.o: buffer.c
	$(CC) $(FLAGS) $?

error.o: error.c
	$(CC) $(FLAGS) $?

index.o: index.c
	$(CC) $(FLAGS) $?

clean:
	rm -f $(OBJS) $(BIN)

count:
	wc $(SOURCES) $(HEADERS)
