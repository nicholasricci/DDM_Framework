CFLAGS=-O2 -lm

ALL: BitmatrixComparator

BitmatrixComparator:	main.o DDM_input_output.o
			cc main.c DDM_input_output.c -o BitmatrixComparator ${CFLAGS}

clean:
	\rm -f *.o *~ BitmatrixComparator 