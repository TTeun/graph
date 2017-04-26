CC = g++
CFLAGS = -std=c++11

all: graph

graph: main.o token.o parser.o expression.o exptree.o
		$(CC) $(CFLAGS) main.o token.o parser.o expression.o exptree.o -o graph

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

clean:
		rm -rf *.o graph
