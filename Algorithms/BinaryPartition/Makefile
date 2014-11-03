BIN=bin/
SEQ=binary_partition_based
OBJSSEQ=DDMBench.o Algorithm.o bitmatrix.o bitvector.o DDM_input_output.o DDMutils.o error.o execution_time.o ExtentMaker.o lists.o

ALL: 			mkdirs release

mkdirs:		
			mkdir -p $(BIN)

release:		CFLAGS=-Wall -O3 -DNDEBUG -Wno-unknown-pragmas -fopenmp -lm
release:		LDLIBS+=-lrt
release:		$(SEQ)

binary_partition_based:	$(OBJSSEQ)
			cc $(CFLAGS) -o $(BIN)binary_partition $(OBJSSEQ)
			
clean:
			rm -f *.o *~ *.txt
			rm -f -R $(BIN)