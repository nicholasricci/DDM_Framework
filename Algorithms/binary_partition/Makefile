BIN=bin/
SEQ=binary_partition
OBJSSEQ=main.o DDM_input_output.o lists.o

ALL: 			mkdirs release

mkdirs:			
			mkdir -p $(BIN)
			
release:		CFLAGS=-Wall -O3 -DNDEBUG -Wno-unknown-pragmas -g
release:		LDLIBS+=-lrt
release:		$(SEQ)

binary_partition:	$(OBJSSEQ)
			cc $(CFLAGS) -o $(BIN)binary_partition $(OBJSSEQ) -lm

clean:
			rm -f *.o
			rm -f -R $(BIN)
