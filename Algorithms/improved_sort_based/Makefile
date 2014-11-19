BIN=bin/
SEQ=improved_sort
OBJSSEQ=main.o DDM_input_output.o

ALL: 			mkdirs release

mkdirs:			
			mkdir -p $(BIN)
			
release:		CFLAGS=-Wall -O3 -DNDEBUG -Wno-unknown-pragmas -g -lm 
release:		LDLIBS+=-lrt
release:		$(SEQ)

improved_sort:	$(OBJSSEQ)
			cc $(CFLAGS) -o $(BIN)improved_sort $(OBJSSEQ) -lm -g

clean:
			rm -f *.o
			rm -f -R $(BIN)