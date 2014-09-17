CFLAGS=-Wall
PROGS=brute_force interval_tree

ALL: release

debug: CFLAGS+=-ggdb
debug: LDLIBS+=-lrt
debug: $(PROGS)

release: CFLAGS+=-O3 -DNDEBUG -Wno-unknown-pragmas
release: LDLIBS+=-lrt
release: $(PROGS)

openmp: CFLAGS+=-O3 -DNDEBUG -fopenmp 
openmp: LDLIBS+=-lgomp -lrt
openmp: $(PROGS)

brute_force: main.o interval.o brute_force.o timing.o bitmatrix.o

interval_tree: main.o interval.o int_tree.o timing.o bitmatrix.o

main.o: main.c cflags.h

cflags.h:
	@echo "const char* CFLAGS=\"$(CFLAGS)\";" > cflags.h

clean:
	\rm -f *.o *~ brute_force interval_tree cflags.h *.txt
