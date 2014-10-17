#!/bin/bash

####################################################################
source configuration.sh
####################################################################

mkdir -p _results
mkdir -p _graphs

rm -f _graphs/speedup_*.txt


for ALG in $PAR_ALGORITHMS
do
	for ALFA in $ALFAS_PAR
	do
		FILENAME_SEQ="exec_time_"$ALG"_alfa_$ALFA.txt"

		EXTENTS=$START_EXTENTS
		COUNTER=1

		while [ $EXTENTS -le "$MAX_EXTENTS" ]
		do

			SEQ_TIME=`cat _graphs/$FILENAME_SEQ | cut -f 2 | head -n $COUNTER | tail -n 1`

			for CORE in {2..12}
			do
				FILENAME_PAR="exec_time_"$ALG"_MP_cores_"$CORE"_alfa_$ALFA.txt"
				PAR_TIME=`cat _graphs/$FILENAME_PAR | cut -f 2 | head -n $COUNTER | tail -n 1`

				SPEEDUP=$(echo "scale=4; $SEQ_TIME/$PAR_TIME" | bc)

				FILENAME_OUT_EXTENTS="speedup_"$ALG"_MP_cores_"$CORE"_alfa_$ALFA.txt"
				echo -e "$EXTENTS\t$SPEEDUP" >> _graphs/$FILENAME_OUT_EXTENTS

				FILENAME_OUT_CORES="speedup_"$ALG"_MP_alfa_"$ALFA"_extents_$EXTENTS.txt"
				echo -e "$CORE\t$SPEEDUP" >> _graphs/$FILENAME_OUT_CORES
			done

			let EXTENTS+=$STEP_SIZE
			let COUNTER+=1
		done
	done
done
