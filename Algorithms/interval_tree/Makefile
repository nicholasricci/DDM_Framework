SEQ=brute_force interval_tree
PAR=brute_force_mp interval_tree_mp
BIN=bin/
OBJSSEQ=main.o interval.o brute_force.o timing.o bitmatrix.o int_tree.o
OBJSPAR=main_mp.o interval_mp.o brute_force_mp.o timing_mp.o bitmatrix_mp.o int_tree_mp.o
OBJSSEQBRUTE=main.o interval.o brute_force.o timing.o bitmatrix.o
OBJSSEQINTTREE=main.o interval.o int_tree.o timing.o bitmatrix.o interval_tree.o
OBJSPARBRUTE=main_mp.o interval_mp.o brute_force_mp.o timing_mp.o bitmatrix_mp.o
OBJSPARINTTREE=main_mp.o interval_mp.o int_tree_mp.o timing_mp.o bitmatrix_mp.o interval_tree_mp.o


ALL: 			mkdirs release openmp

mkdirs:		
			mkdir -p $(BIN)

release:		CFLAGS=-Wall -O3 -DNDEBUG -Wno-unknown-pragmas
release:		LDLIBS+=-lrt
release:		$(SEQ)

openmp:			CFLAGS=-Wall -O3 -DNDEBUG -fopenmp 
openmp:			LDLIBS+=-lgomp
openmp:			$(PAR)

brute_force:		$(OBJSSEQBRUTE)
			cc $(CFLAGS) -o $(BIN)brute_force $(OBJSSEQBRUTE)
		
interval_tree:		$(OBJSSEQINTTREE)
			cc $(CFLAGS) -o $(BIN)interval_tree $(OBJSSEQINTTREE)

brute_force_mp:		$(OBJSPARBRUTE)
			cc $(CFLAGS) -o $(BIN)brute_force_mp $(OBJSPARBRUTE)

interval_tree_mp:	$(OBJSPARINTTREE)
			cc $(CFLAGS) -o $(BIN)interval_tree_mp $(OBJSPARINTTREE)

main.o:			main.c cflags.h

main_mp.o:		main.c cflags.h
			cc $(CFLAGS) -c main.c -o main_mp.o

interval_mp.o:		interval.c interval.h
			cc $(CFLAGS) -c interval.c -o interval_mp.o

brute_force_mp.o:	brute_force.c interval.h bitmatrix.h
			cc $(CFLAGS) -c brute_force.c -o brute_force_mp.o

timing_mp.o:		timing.c timing.h
			cc $(CFLAGS) -c timing.c -o timing_mp.o

bitmatrix_mp.o:		bitmatrix.c bitmatrix.h
			cc $(CFLAGS) -c bitmatrix.c -o bitmatrix_mp.o

int_tree_mp.o:		int_tree.c int_tree.h
			cc $(CFLAGS) -c int_tree.c -o int_tree_mp.o
			
interval_tree_mp.o:	interval_tree.c int_tree.h bitmatrix.h timing.h
			cc $(CFLAGS) -c interval_tree.c -o interval_tree_mp.o

cflags.h:
			@echo "const char* CFLAGS=\"$(CFLAGS)\";" > cflags.h

clean:
			rm -f *.o *~ brute_force interval_tree cflags.h *.txt
			rm -f -R $(BIN)
