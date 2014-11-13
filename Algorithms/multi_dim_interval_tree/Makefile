SEQ=interval_tree
PAR=interval_tree_mp
BIN=bin/
OBJSSEQ=main.o interval.o int_tree.o DDM_input_output.o
OBJSPAR=main_mp.o interval_mp.o int_tree_mp.o DDM_input_output.o


ALL: 			mkdirs release openmp

mkdirs:		
			mkdir -p $(BIN)

release:		CFLAGS=-Wall -O3 -DNDEBUG -Wno-unknown-pragmas -g
release:		LDLIBS+=-lrt
release:		$(SEQ)

openmp:			CFLAGS=-Wall -O3 -DNDEBUG -fopenmp -g
openmp:			LDLIBS+=-lgomp
openmp:			$(PAR)
		
interval_tree:		$(OBJSSEQ)
			cc $(CFLAGS) -o $(BIN)interval_tree $(OBJSSEQ) -lm -g

interval_tree_mp:	$(OBJSSEQ)
			cc $(CFLAGS) -o $(BIN)interval_tree_mp $(OBJSSEQ) -lm

main.o:			main.c

main_mp.o:		main.c DDM_input_output.h
			cc $(CFLAGS) -c main.c -o main_mp.o

interval_mp.o:		interval.c interval.h DDM_input_output.h
			cc $(CFLAGS) -c interval.c -o interval_mp.o

int_tree_mp.o:		int_tree.c int_tree.h DDM_input_output.h
			cc $(CFLAGS) -c int_tree.c -o int_tree_mp.o
			
interval_tree_mp.o:	interval_tree.c int_tree.h DDM_input_output.h
			cc $(CFLAGS) -c interval_tree.c -o interval_tree_mp.o

DDM_input_output.o:	DDM_input_output.c DDM_input_output.h
			cc $(CFLAGS) -c DDM_input_output.c -o DDM_input_output.o

clean:
			rm -f *.o *~ interval_tree *.txt
			rm -f -R $(BIN)
