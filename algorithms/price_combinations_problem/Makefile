CC = g++
CFLAGS = -g -Wall -std=c++11 -O2

TARGETS = main.cpp priceCombinationFinder.cpp

all: main.o
	$(CC) $(CFLAGS) -o main.o main

main.o:	priceCombinationFinder.hpp main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	$(RM) main