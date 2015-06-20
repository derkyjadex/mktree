CFLAGS += -Wall -std=c99

ifeq ($(shell uname),Linux)
  CFLAGS += -D_GNU_SOURCE
  LDLIBS += -lm -pthread -lrt -ldl
endif

all: tree.svg treeserver

tree.o: tree.c tree.h
mktree.o: mktree.c tree.h
civetweb.o: civetweb.c md5.inl civetweb.h
treeserver.o: treeserver.c tree.h civetweb.h

mktree: tree.o mktree.o
treeserver: treeserver.o tree.o civetweb.o

tree.svg: mktree
	./mktree > tree.svg

clean:
	rm -f *.o mktree treeserver tree.svg

.PHONY: all clean

