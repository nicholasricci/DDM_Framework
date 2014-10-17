#!/bin/bash

for CORES in {2..12}
do
	export OMP_NUM_THREADS=$CORES

	./batch-tests_brute_mp.sh $CORES
	./batch-tests_interval_mp.sh $CORES
done

unset OMP_NUM_THREADS
