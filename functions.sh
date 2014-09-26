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

function run_executable_sequential {

  local filename
  local filename_result
  local executed_filename
  local EXTENTS
  
  local RESULTS="../../../_results"
  local GRAPHS="../../../_graphs"
  local AVERAGER="../../../utils/averager"
  
  mkdir -p $RESULTS
  mkdir -p $GRAPHS
  
  for ALFA in $ALFAS
  do
  
	  EXTENTS=$START_EXTENTS
	  filename="$1.txt"
	  executed_filename="exec_time_$1_alfa_$ALFA.txt"
	  
	  while [ $EXTENTS -le "$MAX_EXTENTS" ]
	  do

		  #for R in {1..$RUN}
		  for ((R=1; R<=$RUN; R++))
		  do
			  echo "running >$1< $EXTENTS/$ALFA:$R"
			  ./$1 $EXTENTS $DIMENSION $ALFA
		  done
		  
		  AVERAGE=`$AVERAGER $filename`
		  echo -e "$EXTENTS\t$AVERAGE" >> $executed_filename
		  
		  filename_result="${1}_${EXTENTS}_${DIMENSION}_${ALFA}.txt"
		  
		  mv $filename $RESULTS/$filename_result

		  let EXTENTS+=$STEP_SIZE
		  
	  done

	  mv $executed_filename $GRAPHS
  done

}

function run_executable_parallel {
  local filename
  local filename_result
  local executed_filename
  local EXTENTS
  
  local RESULTS="../../../_results"
  local GRAPHS="../../../_graphs"
  local AVERAGER="../../../utils/averager"
  
  mkdir -p $RESULTS
  mkdir -p $GRAPHS
  
  #for CORE in {2..$CORES}
  for ((CORE=2; CORE<=$CORES; CORE++))
  do
  
    export OMP_NUM_THREADS=$CORE
  
    for ALFA in $ALFAS_PAR
    do
    
	    EXTENTS=$START_EXTENTS
	    filename="$1.txt"
	    executed_filename="exec_time_${1}_cores_${CORE}_alfa_${ALFA}.txt"
	    
	    while [ $EXTENTS -le "$MAX_EXTENTS" ]
	    do

		    #for R in {1..$RUN}
		    for ((R=1; R<=$RUN; R++))
		    do
			    echo "running >$1< $EXTENTS/$ALFA:$R"
			    ./$1 $EXTENTS $DIMENSION $ALFA
		    done
		    
		    AVERAGE=`$AVERAGER $filename`
		    echo -e "$EXTENTS\t$AVERAGE" >> $executed_filename
		    
		    filename_result="${1}_${EXTENTS}_${DIMENSION}_${CORE}_${ALFA}.txt"
		    
		    mv $filename $RESULTS/$filename_result

		    let EXTENTS+=$STEP_SIZE
		    
	    done

	    mv $executed_filename $GRAPHS
    done
    
  done
  
  unset OMP_NUM_THREADS
}