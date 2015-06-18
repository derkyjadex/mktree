SRCS := mktree.c
CFLAGS += -Wall -std=c11

all: tree.svg

mktree: $(SRCS)

tree.svg: mktree
	./mktree > tree.svg

.PHONY: all

