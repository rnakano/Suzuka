CC ?= gcc
NAMES = main
OBJS  = $(addsuffix .o, $(NAMES))
CFLAGS = -O0 -Wall

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o main $^

clean:
	rm -f *.o

.c.o:
	$(CC) $(CFLAGS) -c $<
