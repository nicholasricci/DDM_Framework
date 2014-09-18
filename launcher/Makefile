CC=gcc
CFLAGS=-O2 -Wall
FLAGEXE=-o
FLAGOBJ=-c

BIN=bin/
SRC=src/
INC=inc/

header = $(INC)comparison.h $(INC)inputoutput.h $(INC)launcher.h
source = $(SRC)comparison.c $(SRC)inputoutput.c $(SRC)launcher.c

ALL:			makedirs launcher

makedirs:		
			mkdir -p $(BIN)
			
launcher:		$(header) $(source)
			$(CC) $(CFLAGS) $(FLAGEXE) $(BIN)launcher $(source)			
clean:
			rm -f -R $(BIN)
			rm -f -R $(OBJ)
			




