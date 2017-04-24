CC = g++
CFLAGS = -std=c++11 -Wall -pedantic -Werror -O3


all: graph

graph: main.o token.o parser.o
		$(CC) $(CFLAGS) main.o token.o parser.o -o graph

main.o: main.cc
		$(CC) $(CFLAGS) -c main.cc

parser.o: equation/parser/parser.cc
		$(CC) $(CFLAGS) -c equation/parser/parser.cc

token.o: equation/token.cc
		$(CC) $(CFLAGS) -c equation/token.cc

clean:
		rm -rf *.o graph
