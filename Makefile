UT = unittesting
PFINAL = spath
FINAL = part2_spath
OBJDIR = objects
SRCDIR = sources
LOWDIR = $(SRCDIR)/low_level_structs
LOWDIR_SOURCES = buffer.c index.c intlist.c struct_list.c hash.c visited.c list_pool.c queries.c
LOWDIR_SOURCES_W_PREFIX = $(addprefix $(LOWDIR)/,$(LOWDIR_SOURCES))
HIGHDIR = $(SRCDIR)/graph_and_assistant_structs
HIGHDIR_SOURCES = graph.c CCindex.c scc.c grail.c jobscheduler.c
HIGHDIR_SOURCES_W_PREFIX =  $(addprefix $(HIGHDIR)/,$(HIGHDIR_SOURCES))
UTILDIR = $(SRCDIR)/mains_and_utilities
UTILDIR_SOURCES = main.c testmain.c error.c utils.c main_part2_20161227.c
UTILDIR_SOURCES_W_PREFIX =  $(addprefix $(UTILDIR)/,$(UTILDIR_SOURCES))
HDRDIR = $(SRCDIR)/include
SOURCES_W_PREFIX = $(LOWDIR_SOURCES_W_PREFIX) $(HIGHDIR_SOURCES_W_PREFIX) $(UTILDIR_SOURCES_W_PREFIX)
UTOBJS = buffer.o error.o testmain.o index.o intlist.o struct_list.o hash.o queries.o
UTOBJS_W_PREFIX = $(addprefix $(OBJDIR)/,$(UTOBJS))
PFOBJS = buffer.o error.o main.o index.o intlist.o struct_list.o graph.o visited.o CCindex.o list_pool.o scc.o utils.o grail.o hash.o queries.o jobscheduler.o
PFOBJS_W_PREFIX = $(addprefix $(OBJDIR)/,$(PFOBJS))
FOBJS = buffer.o error.o main_part2_20161227.o index.o intlist.o struct_list.o graph.o visited.o CCindex.o list_pool.o scc.o utils.o grail.o hash.o queries.o
FOBJS_W_PREFIX = $(addprefix $(OBJDIR)/,$(FOBJS))
AOBJS = buffer.o error.o main.o testmain.o index.o intlist.o struct_list.o graph.o list_pool.o visited.o CCindex.o scc.o utils.o grail.o hash.o queries.o main_part2_20161227.o jobscheduler.o
AOBJS_W_PREFIX = $(addprefix $(OBJDIR)/,$(AOBJS))
HEADERS = buffer.h error.h index.h intlist.h struct_list.h hash.h graph.h visited.h list_pool.h scc.h utils.h CCindex.h grail.h queries.h jobscheduler.h
HEADERS_W_PREFIX = $(addprefix $(HDRDIR)/,$(HEADERS))
OPT = -Ofast
CC = gcc
FLAGS = -c -Wall $(OPT)
CHECK_FLAGS = -lcheck -lrt

all :$(PFINAL) $(FINAL) $(UT)

$(FINAL) : $(FOBJS_W_PREFIX)
	$(CC) $(OPT) -o $@ $(FOBJS_W_PREFIX)

$(PFINAL) : $(PFOBJS_W_PREFIX)
	$(CC) $(OPT) -o $@ $(PFOBJS_W_PREFIX) -pthread

$(UT): $(UTOBJS_W_PREFIX)
	$(CC) -o $@ $(UTOBJS_W_PREFIX) $(CHECK_FLAGS)

$(PFOBJS_W_PREFIX) : | $(OBJDIR)

$(OBJDIR) : 
	mkdir -p $@

$(OBJDIR)/graph.o: $(HIGHDIR)/graph.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/main.o: $(UTILDIR)/main.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/queries.o: $(LOWDIR)/queries.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/main_part2_20161227.o: $(UTILDIR)/main_part2_20161227.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/jobscheduler.o: $(HIGHDIR)/jobscheduler.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/grail.o: $(HIGHDIR)/grail.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/list_pool.o : $(LOWDIR)/list_pool.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/visited.o: $(LOWDIR)/visited.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/intlist.o: $(LOWDIR)/intlist.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/CCindex.o: $(HIGHDIR)/CCindex.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/struct_list.o: $(LOWDIR)/struct_list.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/hash.o: $(LOWDIR)/hash.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/testmain.o: $(UTILDIR)/testmain.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/buffer.o: $(LOWDIR)/buffer.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/error.o: $(UTILDIR)/error.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/index.o: $(LOWDIR)/index.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/scc.o: $(HIGHDIR)/scc.c
	$(CC) $(FLAGS) $? -o $@

$(OBJDIR)/utils.o: $(UTILDIR)/utils.c
	$(CC) $(FLAGS) $? -o $@

clean:
	rm -f $(AOBJS_W_PREFIX) $(BIN) $(FINAL) $(PFINAL)

no_exe_clean:
	rm -f $(AOBJS_W_PREFIX)

count:
	wc $(SOURCES_W_PREFIX) $(HEADERS_W_PREFIX)
