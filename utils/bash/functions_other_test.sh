#!/bin/bash 

function read_info_file {
  #$1 is file name
  
  local FILE=$1
  local index=0
  local str
  local line
  while read line; do
      str=${line:0:1}
      if [ "$str" != "#" ];
      then
	let index=index+1
	case "$index" in
	
	  2)_TEST_DIMENSIONS=$line
	    ;;
	  3)_TEST_SUBSCRIPTIONS=$line
	    ;;
	  4)_TEST_UPDATES=$line
	    ;;
	  
	esac
      fi
  done < $FILE
}

function run_other_executable_sequential {
  #$1 is executable name
  #$2 is type test
  #$3 is dimensions
  #$4 is updates
  #$5 is subscriptions
  #$6 is mem for valgrind
  
  local filename
  local filename_result
  
  local RESULTS="../../../Results/$2"
  local GRAPHS="../../../Graphs/$2"
  local AVERAGER="../../../utils/averager"
  
  mkdir -p $RESULTS
  mkdir -p $GRAPHS
  
  filename="$1.txt"
  echo $RUN
  for R in `seq $RUN`
  do
    echo "running >$1< test: $2, dimensions: $3, updates: $4, subscriptions: $5, RUN:$R"
    if [ "$6" = "mem" ];
    then
      
      valgrind --tool=massif --massif-out-file=$_VALGRIND_OUT_FILE ./$1 $2 $3 $4 $5 
      ms_print $_VALGRIND_OUT_FILE > "temp"
      temporaneous_variable=`cat "temp" | head -9 | tail -1 | awk '{print $1}'`
      echo "${temporaneous_variable//^}" > $_VALGRIND_OUT_FILE
      rm -f "temp"
      
    else
			    
      ./$1 $2 $3 $4 $5
	
    fi
      
    let R+=1
  done
  
  if [ "$6" = "dist" ];
  then
    mv $_BITMATRIX_NAME  "$RESULTS/${1}.bin"
  else
    rm $_BITMATRIX_NAME
    if [ "$6" = "mem" ];
    then 
      #average of memory usage
      AVERAGE_MEMORY=`$AVERAGER $_VALGRIND_OUT_FILE`
      echo -e "$AVERAGE_MEMORY" > $_VALGRIND_OUT_FILE
      filename_memory="${1}_${3}_${4}_${5}_$_VALGRIND_FINAL_FILE"
      mv $_VALGRIND_OUT_FILE $RESULTS/$filename_memory
    else
      AVERAGE=`$AVERAGER $filename`
      echo -e "$AVERAGE" > $filename
      filename_result="${1}_${3}_${4}_${5}.txt"
      mv $filename $RESULTS/$filename_result
    fi
  fi
}

function run_other_executable_parallel {
  #$1 is executable name
  #$2 is type test
  #$3 is dimensions
  #$4 is updates
  #$5 is subscriptions
  #$6 is mem for valgrind
  
  local filename
  local filename_result
  
  local RESULTS="../../../Results/$2"
  local GRAPHS="../../../Graphs/$2"
  local AVERAGER="../../../utils/averager"
  
  mkdir -p $RESULTS
  mkdir -p $GRAPHS
  
  filename="$1.txt"
  echo $RUN
  for R in `seq $RUN`
  do
    echo "running >$1< test: $2, dimensions: $3, updates: $4, subscriptions: $5, RUN:$R"
    if [ "$6" = "mem" ];
    then
      
      valgrind --tool=massif --massif-out-file=$_VALGRIND_OUT_FILE ./$1 $2 $3 $4 $5 
      ms_print $_VALGRIND_OUT_FILE > "temp"
      temporaneous_variable=`cat "temp" | head -9 | tail -1 | awk '{print $1}'`
      echo "${temporaneous_variable//^}" > $_VALGRIND_OUT_FILE
      rm -f "temp"
      
    else
			    
      ./$1 $2 $3 $4 $5
	
    fi
      
    let R+=1
  done
  
  if [ "$6" = "dist" ];
  then
    mv $_BITMATRIX_NAME  "$RESULTS/${1}.bin"
  else
    rm $_BITMATRIX_NAME
    if [ "$6" = "mem" ];
    then 
      #average of memory usage
      AVERAGE_MEMORY=`$AVERAGER $_VALGRIND_OUT_FILE`
      echo -e "$AVERAGE_MEMORY" > $_VALGRIND_OUT_FILE
      filename_memory="${1}_${3}_${4}_${5}_$_VALGRIND_FINAL_FILE"
      mv $_VALGRIND_OUT_FILE $RESULTS/$filename_memory
    else
      AVERAGE=`$AVERAGER $filename`
      echo -e "$AVERAGE" > $filename
      filename_result="${1}_${3}_${4}_${5}.txt"
      mv $filename $RESULTS/$filename_result
    fi
  fi
}