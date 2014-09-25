#!/bin/bash

function read_file {
  local FILE=$1
  local index=0
  local str
  local tmparray
  local line
  while read line; do
      str=${line:0:2}
      if [ "$str" != "##" ];
      then
	tmparray[$index]=$line
	let index=index+1
      fi
  done < $FILE
  echo "${tmparray[@]}"
}

function run_executable {

  local filename
  local filename_result
  local executed_filename
  local EXTENTS
  
  local RESULTS="../../../_results"
  local GRAPHS="../../../_graphs"
  local AVERAGER="../../../utils/averager"
  
  mkdir -p $RESULTS
  mkdir -p $GRAPHS
  
  ALFASS=$2
  
  for ALFA in $ALFASS
  do
  
	  EXTENTS=$START_EXTENTS
	  filename="$1.txt"
	  averaged_filename="averaged_$filename"
	  
	  while [ $EXTENTS -le "$MAX_EXTENTS" ]
	  do

		  for R in {1..$RUN}
		  do
			  echo "running >$1< $EXTENTS/$ALFA:$R"
			  ./$1 $EXTENTS $DIMENSION $ALFA
		  done
		  
		  AVERAGE=`$AVERAGER $filename`
		  echo -e "$EXTENTS\t$AVERAGE" >> $averaged_filename
		  
		  filename_result="${1}_${EXTENTS}_${DIMENSION}_${ALFA}.txt"
		  
		  mv $filename $RESULTS/$filename_result

		  let EXTENTS+=$STEP_SIZE
		  
	  done

	  mv $averaged_filename $GRAPHS
  done


}