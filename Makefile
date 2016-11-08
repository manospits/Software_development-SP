BIN=unittesting
FINAL=spath
SOURCES=buffer.c error.c testmain.c index.c intlist.c struct_list.c hash.c graph.c main.c
OBJS=buffer.o error.o testmain.o index.o intlist.o struct_list.o hash.o
FOBJS=buffer.o error.o main.o index.o intlist.o struct_list.o hash.o graph.o
AOBJS=buffer.o error.o main.o testmain.o index.o intlist.o struct_list.o hash.o graph.o
HEADERS=buffer.h error.h index.h intlist.h struct_list.h hash.h graph.h
CC=gcc
FLAGS = -c -Wall
CHECK = -lcheck -lrt

all : $(FINAL) $(BIN)


$(FINAL) : $(FOBJS)
	$(CC) -o $@ $(FOBJS)

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CHECK)

main.o: main.c
	$(CC) $(FLAGS) $?

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
	rm -f $(AOBJS) $(BIN) $(FINAL)

count:
	wc $(SOURCES) $(HEADERS)
