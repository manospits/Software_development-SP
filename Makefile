BIN=testmain
SOURCES=buffer.c error.c testmain.c index.c intlist.c struct_list.c hash.c
OBJS=buffer.o error.o testmain.o index.o intlist.o struct_list.o hash.o
HEADERS=buffer.h error.h index.h intlist.h struct_list.h hash.h
CC=gcc
FLAGS = -c -Wall
CHECK = -lcheck -lrt

$(BIN): $(OBJS)
	$(CC) -g -o $@ $(OBJS) $(CHECK)

intlist.o: intlist.c
	$(CC) $(FLAGS) $?

struct_list.o: struct_list.c
	$(CC) $(FLAGS) $?

hash.o: hash.c
	$(CC) $(FLAGS) $?

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
