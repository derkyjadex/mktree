CFLAGS += -Wall -std=c11

all: tree.svg

tree.o: tree.c tree.h
mktree.o: mktree.c tree.h

mktree: tree.o mktree.o

tree.svg: mktree
	./mktree > tree.svg

clean:
	rm -f *.o mktree tree.svg

.PHONY: all clean

