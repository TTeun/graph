CC = g++
CFLAGS = -g -std=c++14 -Wall -Werror -pedantic

all: graph

graph: main.o token.o parser.o exptree.o  eqmaps.o simplify.o node_util.o diff.o
		$(CC) $(CFLAGS) main.o token.o parser.o exptree.o eqmaps.o node_util.o simplify.o diff.o -o graph

main.o: main.cc
		$(CC) $(CFLAGS) -c main.cc

parser.o: equation/parser/parser.cc equation/parser/parser.h
		$(CC) $(CFLAGS) -c equation/parser/parser.cc

token.o: equation/token.cc equation/token.h
		$(CC) $(CFLAGS) -c equation/token.cc

exptree.o: equation/exptree/exptree.cc equation/exptree/exptree.h
		$(CC) $(CFLAGS) -c equation/exptree/exptree.cc

eqmaps.o: equation/eqmaps/eqmaps.cc equation/eqmaps/eqmaps.h
		$(CC) $(CFLAGS) -c equation/eqmaps/eqmaps.cc

diff.o: equation/exptree/utils/diff.cc equation/exptree/utils/diff.h
		$(CC) $(CFLAGS) -c equation/exptree/utils/diff.cc

node_util.o: equation/exptree/utils/node_util.cc equation/exptree/utils/node_util.h
		$(CC) $(CFLAGS) -c equation/exptree/utils/node_util.cc


simplify.o: equation/exptree/utils/simplify.cc equation/exptree/utils/simplify.h
		$(CC) $(CFLAGS) -c equation/exptree/utils/simplify.cc

clean:
		rm -rf *.o graph
