#!/bin/bash

####################################################################
#
EXE="brute_force_mp"
EXECUTABLE="_bin/$EXE"
#
####################################################################
source configuration.sh
####################################################################

mkdir -p _results
mkdir -p _graphs

for ALFA in $ALFAS_PAR
do
	FILENAME="exec_time_BRUTE_MP_cores_$1_alfa_$ALFA.txt"
	touch $FILENAME

	EXTENTS=$START_EXTENTS

	while [ $EXTENTS -le "$MAX_EXTENTS" ]
	do

		for RUN in {1..30}
		do
			echo "running >$EXE< $EXTENTS/$ALFA:$RUN"
			./$EXECUTABLE $EXTENTS 1 $ALFA
		done

		AVERAGE=`utils/averager brute_force.txt`
		echo -e "$EXTENTS\t$AVERAGE" >> $FILENAME

		mv brute_force.txt _results/$EXE\_$EXTENTS\_1\_$1\_$ALFA.txt

		let EXTENTS+=$STEP_SIZE
		
	done

	mv $FILENAME _graphs/
done
