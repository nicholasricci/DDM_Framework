BIN=bin/
SEQ=binary_partition
OBJSSEQBRUTE=main.o bitmatrix.o bitvector.o DDM_input_output.o DDMutils.o error.o execution_time.o ExtentMaker.o lists.o

ALL: 			mkdirs release

mkdirs:		
			mkdir -p $(BIN)

release:		CFLAGS=-Wall -O3 -DNDEBUG -Wno-unknown-pragmas -fopenmp
release:		LDLIBS+=-lrt
release:		$(SEQ)

binary_partition:	$(OBJSSEQBRUTE)
			cc $(CFLAGS) -o $(BIN)binary_partition $(OBJSSEQBRUTE)
			
clean:
			rm -f *.o *~ brute_force interval_tree cflags.h *.txt
			rm -f -R $(BIN)