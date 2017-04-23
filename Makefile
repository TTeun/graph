CC = g++
CFLAGS = -std=c++11 -Wall -Werror


all: graph

graph: main.o parser.o
		$(CC) $(CFLAGS) main.o parser.o -o graph

main.o: main.cc
		$(CC) $(CFLAGS) -c main.cc

parser.o: equation/parser/parser.cc
		$(CC) $(CFLAGS) -c equation/parser/parser.cc

clean:
		rm -rf *.o graph
