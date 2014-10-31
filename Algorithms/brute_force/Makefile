BIN=bin/
SEQ=multi_dim_brute_force multi_dim_brute_force_mp
OBJSSEQ=main.o DDM_input_output.o
OBJSPAR=main.o DDM_input_output.o

ALL: 			mkdirs release openmp

mkdirs:			
			mkdir -p $(BIN)
			
release:		CFLAGS=-Wall -O3 -DNDEBUG -Wno-unknown-pragmas
release:		LDLIBS+=-lrt
release:		$(SEQ)

openmp:			CFLAGS=-Wall -O3 -DNDEBUG -fopenmp 
openmp:			LDLIBS+=-lgomp
openmp:			$(PAR)

multi_dim_brute_force:	$(OBJSSEQ)
			cc $(CFLAGS) -o $(BIN)multi_dim_brute_force $(OBJSSEQ)
			
multi_dim_brute_force_mp:	$(OBJSPAR)
				cc $(CFLAGS) -o $(BIN)multi_dim_brute_force_mp $(OBJSPAR)

clean:
			rm -f *.o
			rm -f -R $(BIN)