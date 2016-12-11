BIN=unittesting
FINAL=spath
SOURCES=buffer.c error.c testmain.c index.c intlist.c struct_list.c graph.c hash.c main.c visited.c CCindex.c list_pool.c scc.c utils.c grail.c
OBJS=buffer.o error.o testmain.o index.o intlist.o struct_list.o hash.o
FOBJS=buffer.o error.o main.o index.o intlist.o struct_list.o graph.o visited.o CCindex.o list_pool.o scc.o utils.o grail.o
AOBJS=buffer.o error.o main.o testmain.o index.o intlist.o struct_list.o graph.o list_pool.o visited.o CCindex.o scc.o utils.o grail.o
HEADERS=buffer.h error.h index.h intlist.h struct_list.h hash.h graph.h visited.h list_pool.h scc.h utils.h
OPT= -Ofast
CC=gcc
FLAGS = -c -Wall $(OPT)
CHECK = -lcheck -lrt

all : $(FINAL) $(BIN)


$(FINAL) : $(FOBJS)
	$(CC) $(OPT) -o $@ $(FOBJS)

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CHECK)

graph.o: graph.c
	$(CC) $(FLAGS) $?

main.o: main.c
	$(CC) $(FLAGS) $?

grail.o: grail.c
	$(CC) $(FLAGS) $?

list_pool.o : list_pool.c
	$(CC) $(FLAGS) $?

visited.o: visited.c
	$(CC) $(FLAGS) $?

intlist.o: intlist.c
	$(CC) $(FLAGS) $?

CCindex.o: CCindex.c
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

scc.o: scc.c
	$(CC) $(FLAGS) $?

utils.o: utils.c
	$(CC) $(FLAGS) $?

clean:
	rm -f $(AOBJS) $(BIN) $(FINAL)

count:
	wc $(SOURCES) $(HEADERS)
