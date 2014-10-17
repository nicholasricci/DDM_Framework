#!/bin/bash

####################################################################
#
EXE="sort_matching_standard.amd64.Release"
EXECUTABLE="_bin/$EXE"
#
####################################################################
source configuration.sh
####################################################################

mkdir -p _results
mkdir -p _graphs

for ALFA in $ALFAS
do
	FILENAME="exec_time_SORT_alfa_$ALFA.txt"
	touch $FILENAME

	EXTENTS=$START_EXTENTS

	while [ $EXTENTS -le "$MAX_EXTENTS" ]
	do

		for RUN in {1..30}
		do
			echo "running >$EXE< $EXTENTS/$ALFA:$RUN"
			./$EXECUTABLE $EXTENTS 1 $ALFA
		done

		AVERAGE=`utils/averager $EXE\_$EXTENTS\_1\_$ALFA.txt`
		echo -e "$EXTENTS\t$AVERAGE" >> $FILENAME

		mv $EXE\_$EXTENTS\_1\_$ALFA.txt _results/

		let EXTENTS+=$STEP_SIZE
		
	done

	mv $FILENAME _graphs/
done
