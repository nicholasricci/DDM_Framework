#!/bin/bash

echo "Launcher starts!"
echo "Compile and create executables for all algorithms in Algorithms folder"
cd Algorithms
algs=($(ls -d */))
for i in ${algs[*]}
do
  cd $i
  if [ ! -z "$1" ] && [ "$1" == "clean" ];
  then
    make clean
  else
    make
  fi
  cd ..
done
cd ..

echo "Come back to root folder"

echo "Move to launcher folder"
cd launcher
#Check first parameter if insterted, if "clean" string then use make clean command
if [ ! -z "$1" ] && [ "$1" == "clean" ];
then
  make clean
fi

if [ ! -f ./bin/launcher ];
then
  echo "Clean junk"
  make clean
  echo "Compiling Makefile"
  make
fi
echo "Execute launcher in c"

if [ ! -z "$1" ] && [ "$1" != "clean" ];
then
  ./bin/launcher "$1"
else
  ./bin/launcher
fi