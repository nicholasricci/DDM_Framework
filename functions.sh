#!/bin/bash

function read_file {
  index=0
  while read line; do
      tmparray[$index]=$line
  done < $1
  echo "${tmparray[@]}"
}
