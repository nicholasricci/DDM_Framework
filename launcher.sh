#!/bin/bash

#import utilities functions
source functions.sh

function build {

  #Build all algorithms in Algorithms folder
  echo "build"
  cd Algorithms
  algs=($(ls -d */))
  for i in ${algs[*]}
  do
    cd $i
      
    make
    
    cd ..
  done
  cd ..
  
  #Build the utils folder which there is avarage programs
  cd utils
  make
  cd ..
  
}

function configure {

  echo "configure"
  #Creating a configure file that contains the number of extents the programs must starts,
  #the number of max extents that programs must reach,
  #the alfas and alfas for parallel execution
  
  rm -f configure.sh
  touch configure.sh
  echo "creating configuration file"
  echo "#Please don't edit this file manually." > configure.sh
  
  #START_EXTENTS
  while true; do
    read -p "Insert the number of EXTENTS that programs must START:" yn
    if [[ $yn =~ $re_integer ]];
    then
      START_EXTENTS=$yn
      echo "START_EXTENTS=$yn" >> configure.sh
      break
    elif [ -z "$yn" ];
    then
      START_EXTENTS=50000
      echo "START_EXTENTS=$START_EXTENTS" >> configure.sh
      break
    else
      echo "Please insert an integer number."
    fi
  done
  
  #MAX_EXTENTS
  while true; do
    read -p "Insert the number of MAX EXTENTS that programs must reach:" yn
    if [[ $yn =~ $re_integer  && $START_EXTENTS < $yn ]];
    then
      MAX_EXTENTS=$yn
      echo "MAX_EXTENTS=$yn" >> configure.sh
      break
    elif [ -z "$yn" ];
    then
      MAX_EXTENTS=500000
      echo "MAX_EXTENTS=$MAX_EXTENTS" >> configure.sh
      break
    else
      echo "Please insert an integer number."
    fi
  done
  
  #STEP_SIZE
  while true; do
    read -p "Insert the number of STEP SIZE that used for increment START EXTENTS until reach MAX EXTENTS:" yn
    if [[ $yn =~ $re_integer  && $MAX_EXTENTS > $yn ]];
    then
      STEP_SIZE=$yn
      echo "STEP_SIZE=$yn" >> configure.sh
      break
    elif [ -z "$yn" ];
    then
      STEP_SIZE=50000
      echo "STEP_SIZE=$STEP_SIZE" >> configure.sh
      break
    else
      echo "Please insert an integer number."
    fi
  done
  
  #ALFAS
  index=0
  while true; do
    read -p "Insert the $index value of ALFAS:" yn
    if [[ $yn =~ $re_float ]];
    then
      ALFAS[$index]=$yn
      let "index=index+1"
    elif [ -z "$yn" ] && [ $index -eq 0 ];
    then
      ALFAS[$index]=0.001
      break
    elif [ -z "$yn" ];
    then
      break
    else
      echo "Please insert a float number."
    fi
  done
  echo "ALFAS=\"${ALFAS[@]}\"" >> configure.sh
  
  #ALFAS_PAR
  index=0
  while true; do
    read -p "Insert the $index value of ALFAS_PAR:" yn
    if [[ $yn =~ $re_float ]];
    then
      ALFAS_PAR[$index]=$yn
      let "index=index+1"
    elif [ -z "$yn" ] && [ $index -eq 0 ];
    then
      ALFAS_PAR[$index]=100.000
      break
    elif [ -z "$yn" ];
    then
      break
    else
      echo "Please insert a float number."
    fi
  done
  echo "ALFAS_PAR=\"${ALFAS_PAR[@]}\"" >> configure.sh
  
  #RUN
  while true; do
    read -p "Insert the number of RUN you want to programs will do:" yn
    if [[ $yn =~ $re_integer  && $yn -ge 1 ]];
    then
      RUN=$yn
      echo "RUN=$yn" >> configure.sh
      break
    elif [ -z "$yn" ];
    then
      RUN=30
      echo "RUN=$RUN" >> configure.sh
      break
    else
      echo "Please insert an integer number."
    fi
  done

}

function clean {

  echo "clean"
  #clean all algorithms
  cd Algorithms
  algs=($(ls -d */))
  for i in ${algs[*]}
  do
    cd $i
    
    echo -e "\n$i"
    make clean
    
    cd ..
  done
  cd ..
  
  #clean utils
  cd utils
  echo -e "\nutils"
  make clean
  cd ..
  
  #Unset all variable
  unset $START_EXTENTS
  unset $MAX_EXTENTS
  unset $STEP_SIZE
  unset $ALFAS
  unset $ALFAS_PAR
  unset $RUN

}

function run {

  echo "run"

}

function check_configure {
  
  #import configure.sh
  source configure.sh
  
  #check if configure.sh contains values, if not contains call configure function
  if [ -z "$START_EXTENTS" ] || [ -z "$MAX_EXTENTS" ] || [ -z "$STEP_SIZE" ] || [ -z "$ALFAS" ] || [ -z "$ALFAS_PAR" ] || [ -z "$RUN" ]
  then
    configure
  fi
  
}

#regex for integer and float
re_integer='^[0-9]+$'
re_float='^[0-9]+([.][0-9]+)?$'

if [ ! -z "$1" ] && [ "$1" == "--help" ];
then

  echo "for build and run all algorithms:
usage: 	./launcher.sh EXTENTS DIMENSION ALFA
  
for only build:
usage:	./launcher.sh build

for only run:
usage:	./launcher.sh run

for clean all builded objects:
usage:	./launcher.sh clean

for configure the run command:
usage:	./launcher.sh configure"

elif [ ! -z "$1" ] && [ "$1" == "build" ];
then
 
  build
 
elif [ ! -z "$1" ] && [ "$1" == "run" ];
then

  run
  
elif [ ! -z "$1" ] && [ "$1" == "clean" ];
then

  clean
  
elif [ ! -z "$1" ] && [ "$1" == "configure" ];
then

  configure
  
else

  if [[ $1 =~ $re_integer && $2 =~ $re_integer && $3 =~ $re_float ]];
  then
 
    check_configure
    build
    
  else
  
    echo "usage: ./launcher.sh INTEGER INTEGER FLOAT"
  
  fi

fi