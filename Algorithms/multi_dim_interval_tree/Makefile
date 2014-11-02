SEQ=multi_dim_interval_tree
PAR=multi_dim_interval_tree_mp
BIN=bin/
OBJSSEQ=main.o interval.o bitmatrix.o int_tree.o interval_tree.o DDM_input_output.o
OBJSPAR=main_mp.o interval_mp.o bitmatrix_mp.o int_tree_mp.o interval_tree_mp.o DDM_input_output.o


ALL: 			mkdirs release openmp

mkdirs:		
			mkdir -p $(BIN)

release:		CFLAGS=-Wall -O3 -DNDEBUG -Wno-unknown-pragmas
release:		LDLIBS+=-lrt
release:		$(SEQ)

openmp:			CFLAGS=-Wall -O3 -DNDEBUG -fopenmp 
openmp:			LDLIBS+=-lgomp
openmp:			$(PAR)
		
multi_dim_interval_tree:		$(OBJSSEQ)
			cc $(CFLAGS) -o $(BIN)multi_dim_interval_tree $(OBJSSEQ)

multi_dim_interval_tree_mp:	$(OBJSPAR)
			cc $(CFLAGS) -o $(BIN)multi_dim_interval_tree_mp $(OBJSPAR)

main.o:			main.c cflags.h

main_mp.o:		main.c cflags.h
			cc $(CFLAGS) -c main.c -o main_mp.o

interval_mp.o:		interval.c interval.h
			cc $(CFLAGS) -c interval.c -o interval_mp.o

bitmatrix_mp.o:		bitmatrix.c bitmatrix.h
			cc $(CFLAGS) -c bitmatrix.c -o bitmatrix_mp.o

int_tree_mp.o:		int_tree.c int_tree.h
			cc $(CFLAGS) -c int_tree.c -o int_tree_mp.o
			
interval_tree_mp.o:	interval_tree.c int_tree.h bitmatrix.h
			cc $(CFLAGS) -c interval_tree.c -o interval_tree_mp.o

DDM_input_output.o:	DDM_input_output.c DDM_input_output.h
			cc $(CFLAGS) -c DDM_input_output.c -o DDM_input_output.o

cflags.h:
			@echo "const char* CFLAGS=\"$(CFLAGS)\";" > cflags.h

clean:
			rm -f *.o *~ interval_tree cflags.h *.txt
			rm -f -R $(BIN)
