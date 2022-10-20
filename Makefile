CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -g
MAIN = tree
OBJS = tree.o

all : $(MAIN)

$(MAIN) : $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

tree.o : tree.c
	$(CC) $(CFLAGS) -c tree.c