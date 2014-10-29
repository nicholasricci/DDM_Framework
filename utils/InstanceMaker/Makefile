CFLAGS=-O2 -fopenmp -lm

ALL: DDMInstanceMaker

DDMInstanceMaker:	InstanceMakerMain.o ExtentMaker.o execution_time.o error.o DDMutils.o  bitmatrix.o lists.o
			cc InstanceMakerMain.c ExtentMaker.c execution_time.c error.c DDMutils.c  bitmatrix.c lists.c  -o DDMInstanceMaker ${CFLAGS}

clean:
	\rm -f *.o *~ DDMInstanceMaker 
