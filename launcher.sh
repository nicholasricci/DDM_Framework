#!/bin/bash

if [ ! -z "$1" ] && [ "$1" == "--help" ];
then

  echo "for build and run all algorithms:
usage: 	./launcher.sh EXTENTS DIMENSIONS ALFA
  
for only build:
usage:	./launcher.sh build

for only run:
usage:	./launcher.sh run

for clean all builded objects:
usage:	./launcher.sh clean"

fi

if [ ! -z "$1" ] && [ "$1" == "build" ];
then
 
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
 
fi

if [ ! -z "$1" ] && [ "$1" == "run" ];
then

  echo "run"
  
fi

if [ ! -z "$1" ] && [ "$1" == "clean" ];
then

  echo "clean"
  cd Algorithms
  algs=($(ls -d */))
  for i in ${algs[*]}
  do
    cd $i
    
    make clean
    
    cd ..
  done
  cd ..
  
else

  re_integer='^[0-9]+$'
  re_float='^[0-9]+([.][0-9]+)?$'

  if [[ $1 =~ $re_integer && $2 =~ $re_integer && $3 =~ $re_float ]];
  then
  
    echo "ok!"
    #Go inside "Algorithms" folder and for each algorithm execute the command "make" 
    #if the launcher has one argument and is "clean" then execute the command "make clean" for each algorithm
    #it's better if in each algorithm there is a clean target in make file.
    echo "Compile and create executables for all algorithms in Algorithms folder"
    cd Algorithms
    algs=($(ls -d */))
    for i in ${algs[*]}
    do
      cd $i
      
      make
      
      cd ..
    done
    cd ..
    
  else
  
    echo "usage: ./launcher.sh INTEGER INTEGER FLOAT"
  
  fi

fi