#!/bin/bash 

function read_info_file {
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
