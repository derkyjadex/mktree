SRCS := mktree.c
CFLAGS += -Wall -std=c11

all: tree.svg

clean:
	rm -f mktree tree.svg

mktree: $(SRCS)

tree.svg: mktree
	./mktree > tree.svg

.PHONY: all clean

