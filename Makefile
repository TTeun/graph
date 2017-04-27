CC = g++
CFLAGS = -std=c++11 -Wall -Werror

all: graph

graph: main.o token.o parser.o expression.o exptree.o eqmaps.o
		$(CC) $(CFLAGS) main.o token.o parser.o expression.o exptree.o eqmaps.o -o graph

main.o: main.cc
		$(CC) $(CFLAGS) -c main.cc

parser.o: equation/parser/parser.cc
		$(CC) $(CFLAGS) -c equation/parser/parser.cc

token.o: equation/token.cc
		$(CC) $(CFLAGS) -c equation/token.cc

expression.o: equation/expression/expression.cc
		$(CC) $(CFLAGS) -c equation/expression/expression.cc

exptree.o: equation/exptree/exptree.cc
		$(CC) $(CFLAGS) -c equation/exptree/exptree.cc

eqmaps.o: equation/eqmaps/eqmaps.cc
		$(CC) $(CFLAGS) -c equation/eqmaps/eqmaps.cc

clean:
		rm -rf *.o graph
