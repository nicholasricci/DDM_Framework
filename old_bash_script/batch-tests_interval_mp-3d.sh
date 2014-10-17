#!/bin/bash

####################################################################
#
EXE="interval_tree_mp"
EXECUTABLE="_bin/$EXE"
#
####################################################################
source configuration.sh
####################################################################

mkdir -p _results
mkdir -p _graphs

rm -f _graphs/exec_time_INTERVAL_MP_3D.txt

FILENAME="exec_time_INTERVAL_MP_3D.txt"
touch $FILENAME


export OMP_NUM_THREADS=8

for ALFA in {1..100}
do
	EXTENTS=$START_EXTENTS

	while [ $EXTENTS -le "$MAX_EXTENTS" ]
	do

		for RUN in {1..30}
		do
			echo "running >$EXE< $EXTENTS/$ALFA:$RUN"
			./$EXECUTABLE $EXTENTS 1 $ALFA
		done

		AVERAGE=`utils/averager interval_tree.txt`
		echo -e "$ALFA\t$EXTENTS\t$AVERAGE" >> $FILENAME

		rm interval_tree.txt

		let EXTENTS+=$STEP_SIZE
	done
done

mv $FILENAME _graphs/

unset OMP_NUM_THREADS
