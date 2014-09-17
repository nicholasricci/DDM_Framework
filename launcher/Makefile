CC=gcc
CFLAGS=-O2 -Wall
FLAGEXE=-o
FLAGOBJ=-c

BIN=bin/
OBJ=obj/
SRC=src/
INC=inc/

ALL: makedirs comparison inputoutput launcher

makedirs:	
		mkdir $(BIN)
		mkdir $(OBJ)

comparison: 	comparison.c
		$(CC) $(CFLAGS) $(FLAGEXE) $(BIN)comparison comparison.c
		$(CC) $(CFLAGS) $(FLAGOBJ) comparison.c
		mv comparison.o $(OBJ)

inputoutput: 	inputoutput.c
		$(CC) $(CFLAGS) $(FLAGEXE) $(BIN)inputoutput inputoutput.c
		$(CC) $(CFLAGS) $(FLAGOBJ) inputoutput.c
		mv inputoutput.o $(OBJ)

launcher: 	launcher.c
		$(CC) $(CFLAGS) $(FLAGEXE) $(BIN)launcher launcher.c
		$(CC) $(CFLAGS) $(FLAGOBJ) launcher.c
		mv launcher.o $(OBJ)
		
clean:
	rm -f -R $(BIN)
	rm -f -R $(OBJ)

