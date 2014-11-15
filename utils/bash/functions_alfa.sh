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

function create_alfa_folders_and_files {
  #$1 ALFA
  #$2 EXTENTS

  local ALFA_FOLDER="../../../TestsInstances/ALFA_$1_$2_$DIMENSION"
  local INFO="info.txt"
  local INPUT="input-0.txt"
  local ALFA_INFO="$ALFA_FOLDER/$INFO"
  local ALFA_INPUT="$ALFA_FOLDER/$INPUT"
  local ALFA_CREATOR="../../../utils/alfa_creator"
  
  mkdir -p $ALFA_FOLDER
  
  touch $ALFA_INFO
  touch $ALFA_INPUT
  
  echo "#Instance name: ALFA_$1_$2_$DIMENSION" > $ALFA_INFO
  echo "#Instance version: 1" >> $ALFA_INFO
  echo "#Author: Nicholas Ricci" >> $ALFA_INFO
  echo "#Random Seed: XXXX" >> $ALFA_INFO
  echo "#Sequence length:" >> $ALFA_INFO
  echo "1" >> $ALFA_INFO
  echo "#Dimensions" >> $ALFA_INFO
  echo $DIMENSION >> $ALFA_INFO
  SUBSCRIPTIONS=`echo "$2 / 2" | bc`
  echo "#Subscription Regions" >> $ALFA_INFO
  echo "$2 / 2" | bc >> $ALFA_INFO
  UPDATES=`echo "$2 / 2" | bc`
  echo "#Update Regions" >> $ALFA_INFO
  echo "$2 / 2" | bc >> $ALFA_INFO
  
  $ALFA_CREATOR $2 $DIMENSION $1 $ALFA_INPUT
  
  
}

function create_all_alfa_folders_and_files {
  local alfas=( "$@" )
  for ALFA in ${alfas[@]}
  do
    EXTENTS=$START_EXTENTS
    while [ $EXTENTS -le "$MAX_EXTENTS" ]
    do
      create_alfa_folders_and_files $ALFA $EXTENTS
      let EXTENTS+=$STEP_SIZE
    done
  done
}

function run_alfa_executable_sequential {

  #$3 is mem for valgrind

  local filename
  local filename_result
  local executed_filename
  local EXTENTS
  
  local RESULTS="../../../Results/ALFA"
  local GRAPHS="../../../Graphs/ALFA"
  local AVERAGER="../../../utils/averager"
  
  mkdir -p $RESULTS
  mkdir -p $GRAPHS
  
  for ALFA in $ALFAS
  do
  
	  EXTENTS=$START_EXTENTS
	  filename="$1.txt"
	  executed_filename="exec_time_$1_alfa_$ALFA.txt"
	  executed_filename_memory="exec_memory_$1_alfa_${ALFA}_$_VALGRIND_FINAL_FILE"
	  
	  while [ $EXTENTS -le "$MAX_EXTENTS" ]
	  do

		  #for R in {1..$RUN}
		  for ((R=1; R<=$RUN; R++))
		  do
			  echo "running >$1< $EXTENTS/$ALFA:$R"
			  
			  #If VALGRIND VARIABLE IS USED RUN WITH DIFFERENT COMMAND
			  if [ "$3" = "mem" ];
			  then
			  
			    valgrind --tool=massif --massif-out-file=$_VALGRIND_OUT_FILE ./$1 $2 $EXTENTS $DIMENSION $ALFA 
			    ms_print $_VALGRIND_OUT_FILE > "temp"
			    temporaneous_variable=`cat "temp" | head -9 | tail -1 | awk '{print $1}'`
			    echo "${temporaneous_variable//^}" > $_VALGRIND_OUT_FILE
			    rm -f "temp"
			  
			  else
			  
			    ./$1 $2 $EXTENTS $DIMENSION $ALFA 
			  
			  fi
		  done
		  
		  RESULT_FOLDER="${RESULTS}_${ALFA}_${EXTENTS}_${DIMENSION}"
		  mkdir -p $RESULT_FOLDER
		  
		  if [ "$3" = "dist" ];
		  then
		    mv $_BITMATRIX_NAME "$RESULT_FOLDER/${1}.bin"
		  else
		    rm $_BITMATRIX_NAME
		    if [ "$3" != "mem" ];
		    then
		      #average of time
		      AVERAGE=`$AVERAGER $filename`
		      echo -e "$AVERAGE" > $executed_filename
		      filename_result="${1}_${EXTENTS}_${DIMENSION}_${ALFA}.txt"
		      mv $filename $RESULT_FOLDER/$filename_result
		    else
		      #average of memory usage
		      AVERAGE_MEMORY=`$AVERAGER $_VALGRIND_OUT_FILE`
		      echo -e "$AVERAGE_MEMORY" > $executed_filename_memory
		      filename_memory="${1}_${EXTENTS}_${DIMENSION}_${ALFA}_$_VALGRIND_FINAL_FILE"
		      mv $executed_filename_memory $RESULT_FOLDER/$filename_memory
		    fi
		  fi

		  let EXTENTS+=$STEP_SIZE
		  
	  done

	  #mv $executed_filename $GRAPHS
	  #mv $executed_filename_memory $GRAPHS
  done

}

function run_alfa_executable_parallel {
  local filename
  local filename_result
  local executed_filename
  local EXTENTS
  
  local RESULTS="../../../Results/ALFA"
  local GRAPHS="../../../Graphs/ALFA"
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
	    executed_filename_memory="exec_memory_$1_alfa_${ALFA}_$_VALGRIND_FINAL_FILE"
	    
	    while [ $EXTENTS -le "$MAX_EXTENTS" ]
	    do

		    #for R in {1..$RUN}
		    for ((R=1; R<=$RUN; R++))
		    do
			    echo "running >$1< $EXTENTS/$ALFA:$R"
			    #If VALGRIND VARIABLE IS USED RUN WITH DIFFERENT COMMAND
			    if [ "$3" = "mem" ];
			    then
			    
			      valgrind --tool=massif --massif-out-file=$_VALGRIND_OUT_FILE ./$1 $2 $EXTENTS $DIMENSION $ALFA 
			      ms_print $_VALGRIND_OUT_FILE > "temp"
			      temporaneous_variable=`cat "temp" | head -9 | tail -1 | awk '{print $1}'`
			      echo "${temporaneous_variable//^}" > $_VALGRIND_OUT_FILE
			      rm -f "temp"
			    
			    else
			    
			      ./$1 $2 $EXTENTS $DIMENSION $ALFA
			    
			    fi
		    done
		    
		    RESULT_FOLDER="${RESULTS}_${ALFA}_${EXTENTS}_${DIMENSION}"
		    mkdir -p $RESULT_FOLDER
		    
		    if [ "$3" = "dist" ];
		    then
		      mv $_BITMATRIX_NAME "$RESULT_FOLDER/${1}_mp.bin"
		    else
		      rm $_BITMATRIX_NAME
		      if [ "$3" != "mem" ];
		      then
			AVERAGE=`$AVERAGER $filename`
			echo -e "$AVERAGE" > $executed_filename
			filename_result="${1}_mp_${EXTENTS}_${DIMENSION}_${CORE}_${ALFA}.txt"
			mv $filename $RESULT_FOLDER/$filename_result
		      else
			#average of memory usage
			AVERAGE_MEMORY=`$AVERAGER $_VALGRIND_OUT_FILE`
			echo -e "$AVERAGE_MEMORY" > $executed_filename_memory
			filename_memory="${1}_mp_${EXTENTS}_${DIMENSION}_${ALFA}_$_VALGRIND_OUT_FILE"     
			mv $executed_filename_memory $RESULT_FOLDER/$filename_memory
		      fi
		    fi
		    
		    let EXTENTS+=$STEP_SIZE
		    
	    done

	    #mv $executed_filename $GRAPHS
	    #mv $executed_filename_memory $GRAPHS
    done
    
  done
  
  unset OMP_NUM_THREADS
}