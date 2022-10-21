CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -g
MAIN = tree
OBJS = tree.o array_list.o

all : $(MAIN)

$(MAIN) : $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

tree.o : tree.c
	$(CC) $(CFLAGS) -c tree.c

array_list.o : array_list.c array_list.h
	$(CC) $(CFLAGS) -c array_list.c