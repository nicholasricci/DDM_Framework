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
    ./$1 $2 $3 $4 $5 
    let R+=1
  done
  
  AVERAGE=`$AVERAGER $filename`
  echo -e "$AVERAGE" > $filename
  
  filename_result="${1}_${3}_${4}_${5}.txt"
  
  mv $filename $RESULTS/$filename_result
}

function run_other_executable_parallel {
  #$1 is executable name
  #$2 is type test
  #$3 is dimensions
  #$4 is updates
  #$5 is subscriptions
  
  local filename
  local filename_result
  
  local RESULTS="../../../Results/$2"
  local GRAPHS="../../../Graphs/$2"
  local AVERAGER="../../../utils/averager"
  
  mkdir -p $RESULTS
  mkdir -p $GRAPHS
  
  filename="$1.txt"
  
  for R in $(seq $RUN)
  do
    echo "running >$1< test: $2, dimensions: $3, updates: $4, subscriptions: $5, RUN:$R"
    ./$1 $2 $3 $4 $5 
  done
  
  AVERAGE=`$AVERAGER $filename`
  echo -e "$AVERAGE" > $filename
  
  filename_result="${1}_${3}_${4}_${5}.txt"
  
  mv $filename $RESULTS/$filename_result
}