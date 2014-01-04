CC ?= gcc
CPC = g++
NAMES = http log memory server handler file
OBJS  = $(addsuffix .o, $(NAMES))
MAIN  = main.o
CFLAGS = -O0 -Wall -std=c99 -g

all: main

main: $(OBJS) $(MAIN)
	$(CC) $(CFLAGS) -o main $^

clean:
	rm -f *.o

test: test.cpp $(OBJS)
	$(CPC) -Wall -lgtest -o test $^
	./test

.c.o:
	$(CC) $(CFLAGS) -c $<
