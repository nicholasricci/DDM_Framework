#!/bin/bash

#import utilities functions
source ./utils/bash/functions_alfa.sh
source ./utils/bash/functions_other_test.sh
source ./utils/bash/definitions.sh

function build {

  echo "###########################"
  echo "########## BUILD ##########"
  echo "###########################"

  #Build all algorithms in Algorithms folder
  cd $_ALGORITHMS
  algs=($(ls -d */))
  for i in ${algs[*]}
  do
    cd $i
    
    if [ ! -f $_DDM_PARALLEL ];
    then
      touch $_DDM_PARALLEL
      echo "## Insert the name of parallel executables in bin folder. One line, one executable." > $_DDM_PARALLEL
    fi
    if [ ! -f $_DDM_SEQUENTIAL ];
    then
      touch $_DDM_SEQUENTIAL
      echo "## Insert the name of sequential executables in bin folder. One line, one executable." > $_DDM_SEQUENTIAL
    fi
    make
    
    cd ..
  done
  cd ..
  
  #Build the utils folder which there is avarage programs
  cd $_UTILS
  make
    #Build the InstanceMaker folder which there is a program that can create interesting tests
    cd $_INSTANCE_MAKER
    make
    mv ./$_DDM_INSTANCE_MAKER ../
    cd ..
    cd $_F_BITMATRIX_COMPARATOR
    make
    mv ./$_BITMATRIX_COMPARATOR ../
    cd ..
  cd ..
  
}

function configure {

  echo "###########################"
  echo "######## CONFIGURE ########"
  echo "###########################"
  
  #Creating a configure file that contains the number of extents the programs must starts,
  #the number of max extents that programs must reach,
  #the alfas and alfas for parallel execution
  
  rm -R -f $_CONFIGURATION
  mkdir -p $_CONFIGURATION
  touch ./$_CONFIGURATION/$_CONFIGURATION_SHELL
  
  #go to configuration folder
  cd $_CONFIGURATION
  
  echo "creating configuration file"
  echo "#Please don't edit this file manually." > $_CONFIGURATION_SHELL
  
  #START_EXTENTS
  while true; do
    read -p "Insert the number of EXTENTS that programs must START:" yn
    if [[ $yn =~ $re_integer ]];
    then
      START_EXTENTS=$yn
      echo "START_EXTENTS=$yn" >> $_CONFIGURATION_SHELL
      break
    elif [ -z "$yn" ];
    then
      START_EXTENTS=50000
      echo "START_EXTENTS=$START_EXTENTS" >> $_CONFIGURATION_SHELL
      break
    else
      echo "Please insert an integer number."
    fi
  done
  
  #MAX_EXTENTS
  while true; do
    read -p "Insert the number of MAX EXTENTS that programs must reach:" yn
    if [[ $yn =~ $re_integer  && $START_EXTENTS -le $yn ]];
    then
      MAX_EXTENTS=$yn
      echo "MAX_EXTENTS=$yn" >> $_CONFIGURATION_SHELL
      break
    elif [ -z "$yn" ];
    then
      MAX_EXTENTS=500000
      echo "MAX_EXTENTS=$MAX_EXTENTS" >> $_CONFIGURATION_SHELL
      break
    else
      echo "Please insert an integer number."
    fi
  done
  
  #STEP_SIZE
  while true; do
    read -p "Insert the number of STEP SIZE that used for increment START EXTENTS until reach MAX EXTENTS:" yn
    if [[ $yn =~ $re_integer  && $MAX_EXTENTS -ge $yn ]];
    then
      STEP_SIZE=$yn
      echo "STEP_SIZE=$yn" >> $_CONFIGURATION_SHELL
      break
    elif [ -z "$yn" ];
    then
      STEP_SIZE=50000
      echo "STEP_SIZE=$STEP_SIZE" >> $_CONFIGURATION_SHELL
      break
    else
      echo "Please insert an integer number."
    fi
  done
  
  #DIMENSION
  while true; do
    read -p "Insert the number of DIMENSION you want to use:" yn
    if [[ $yn =~ $re_integer  && $yn -ge 1 ]];
    then
      DIMENSION=$yn
      echo "DIMENSION=$yn" >> $_CONFIGURATION_SHELL
      break
    elif [ -z "$yn" ];
    then
      DIMENSION=1
      echo "DIMENSION=$DIMENSION" >> $_CONFIGURATION_SHELL
      break
    else
      echo "Please insert an integer number."
    fi
  done
  
  #CORES
  while true; do
    read -p "Insert the number of max CORES you want to use (>=2):" yn
    if [[ $yn =~ $re_integer  && $yn -ge 2 ]];
    then
      CORES=$yn
      echo "CORES=$yn" >> $_CONFIGURATION_SHELL
      break
    elif [ -z "$yn" ];
    then
      CORES=12
      echo "CORES=$CORES" >> $_CONFIGURATION_SHELL
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
  echo "ALFAS=\"${ALFAS[@]}\"" >> $_CONFIGURATION_SHELL
  
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
  echo "ALFAS_PAR=\"${ALFAS_PAR[@]}\"" >> $_CONFIGURATION_SHELL
  
  #RUN
  while true; do
    read -p "Insert the number of RUN you want to programs will do:" yn
    if [[ $yn =~ $re_integer  && $yn -ge 1 ]];
    then
      RUN=$yn
      echo "RUN=$yn" >> $_CONFIGURATION_SHELL
      break
    elif [ -z "$yn" ];
    then
      RUN=30
      echo "RUN=$RUN" >> $_CONFIGURATION_SHELL
      break
    else
      echo "Please insert an integer number."
    fi
  done

  #Return to root
  cd ..
  
}

function cleantestsinstances {
  
  echo "###########################"
  echo "## CLEAN TESTS INSTANCES ##"
  echo "###########################"
  
  #clean testsinstances
  rm -R -f $_TESTS_INSTANCES
  echo "Removed TestsInstances folder"
}

function cleanutils {

  echo "###########################"
  echo "####### CLEAN UTILS #######"
  echo "###########################"
  
  #clean utils
  cd $_UTILS
  echo -e "\nutils"
  make clean
    cd $_INSTANCE_MAKER
    make clean
    cd ..
  rm -f $_DDM_INSTANCE_MAKER
  cd ..
}

function cleanalgorithms {

  echo "###########################"
  echo "##### CLEAN ALGORITHMS ####"
  echo "###########################"
  
  #clean all algorithms
  cd $_ALGORITHMS
  algs=($(ls -d */))
  for i in ${algs[*]}
  do
    cd $i
    
    echo -e "\n$i"
    make clean
    
    cd ..
  done
  cd ..
  
  #Unset all variable
  unset $START_EXTENTS
  unset $MAX_EXTENTS
  unset $STEP_SIZE
  unset $DIMENSION
  unset $ALFAS
  unset $ALFAS_PAR
  unset $RUN
  
}

function cleanresults {

  echo "###########################"
  echo "###### CLEAN RESULTS ######"
  echo "###########################"

  rm -R -f $_RESULTS
  rm -R -f $_GRAPHS
  echo "Removed $_RESULTS and $_GRAPHS folders"
}

function clean {

  echo "###########################"
  echo "######## CLEAN ALL ########"
  echo "###########################"
  
  cleanalgorithms
  
  cleanresults
  
  cleanutils

  cleantestsinstances
}

function run {

  echo "###########################"
  echo "########### RUN ###########"
  echo "###########################"
  
  check_configure
  build
  
  if  [ $# -ge 1 ] && [ $# -le 2 ] && [ "$1" == "alfa" ]; 
  then
    
    cd $_ALGORITHMS
    algs=($(ls -d */))
    for i in ${algs[*]}
    do
      cd $i
      #get the executables sequential and parallel in this directory
      exe_sequential=$(read_file DDM_Sequential)
      exe_parallel=$(read_file DDM_Parallel)
      #change to bin directory
      cd $_ALGORITHM_BIN
      #prepare alfa folders and files
      create_all_alfa_folders_and_files "${ALFAS[@]}"
      #for each executables sequential starts program in some configuration
      for exe in ${exe_sequential[*]}
      do
	if [ "$2" != "mem" ] && [ "$2" != "dist" ];
	then
	  run_alfa_executable_sequential $exe $1
	else
	  run_alfa_executable_sequential $exe $1 $2
	fi
      done
      #prepare alfa folders and files
      create_all_alfa_folders_and_files "${ALFAS_PAR[@]}"
      #for each executables parallel starts program in some configuration
      for exe_par in ${exe_parallel[*]}
      do
	if [ "$2" != "mem" ] && [ "$2" != "dist" ];
	then
	  run_alfa_executable_parallel $exe_par $1
	else
	  run_alfa_executable_parallel $exe_par $1 $2
	fi
      done
      #change to algorithm directory
      cd ..
      #change to Algoirthms directory
      cd ..
    done
    
    unset exe_sequential
    unset exe_parallel
    
  elif [ $# -ge 1 ] && [ $# -le 2 ] && [ "$1" != "alfa" ];
  then
    
    if [ -d "$_TESTS_INSTANCES" ];
    then
      
      #Change dir to TestsInstances
      cd $_TESTS_INSTANCES
      if [ -d "$1" ];
      then
      
	cd $1
	
	read_info_file "info.txt"
	#After this function is available to use
	#this three global variable
	#$_TEST_DIMENSIONS, _TEST_SUBSCRIPTIONS, _TEST_UPDATES
	
	cd ..
      
      else
      
	echo "The Test Instance you've digit doesn't exist!"
      
      fi
      #Return to root
      cd ..
      
    else
      
      echo "Create some Tests Instances!"
      
    fi
    
    if [ ! -z "$_TEST_DIMENSIONS" ] && [ ! -z "$_TEST_SUBSCRIPTIONS" ] && [ ! -z "$_TEST_UPDATES" ];
    then
    
      cd $_ALGORITHMS
      algs=($(ls -d */))
      for i in ${algs[*]}
      do
	cd $i
	#get the executables sequential and parallel in this directory
	exe_sequential=$(read_file DDM_Sequential)
	exe_parallel=$(read_file DDM_Parallel)
	#change to bin directory
	cd $_ALGORITHM_BIN
	#for each executables sequential starts program in some configuration
	for exe in ${exe_sequential[*]}
	do
	  if [ "$2" != "mem" ] && [ "$2" != "dist" ];
	  then
	    run_other_executable_sequential $exe $1 $_TEST_DIMENSIONS $_TEST_UPDATES $_TEST_SUBSCRIPTIONS
	  else
	    run_other_executable_sequential $exe $1 $_TEST_DIMENSIONS $_TEST_UPDATES $_TEST_SUBSCRIPTIONS $2
	  fi
	done
	#for each executables parallel starts program in some configuration
	for exe_par in ${exe_parallel[*]}
	do
	  if [ "$2" != "mem" ] && [ "$2" != "dist" ];
	  then
	    run_other_executable_parallel $exe $1 $_TEST_DIMENSIONS $_TEST_UPDATES $_TEST_SUBSCRIPTIONS
	  else
	    run_other_executable_parallel $exe $1 $_TEST_DIMENSIONS $_TEST_UPDATES $_TEST_SUBSCRIPTIONS $2
	  fi
	done
	#change to algorithm directory
	cd ..
	#change to Algoirthms directory
	cd ..
      done
      
      unset exe_sequential
      unset exe_parallel
    
    else
    
      echo "Variables for tests isn't set!"
    
    fi
  else
  
    echo " syntax: usage: ./launcher.sh run STRING
meaning: usage: ./launcher.sh run TYPE_TEST
example: usage: ./launcher.sh run alfa"
  
  fi
  
  unset _TEST_DIMENSIONS
  unset _TEST_SUBSCRIPTIONS
  unset _TEST_UPDATES
}

function check_configure {
  
  echo "###########################"
  echo "##### CHECK CONFIGURE #####"
  echo "###########################"
  
  #import configure.sh
  source $_CONFIGURATION/$_CONFIGURATION_SHELL
  
  #check if configure.sh contains values, if not contains call configure function
  if [ -z "$START_EXTENTS" ] || [ -z "$MAX_EXTENTS" ] || [ -z "$STEP_SIZE" ] || [ -z "$ALFAS" ] || [ -z "$ALFAS_PAR" ] \
  || [ -z "$RUN" ] || [ -z "$DIMENSION" ] || [ -z "$CORES" ]
  then
    configure
  fi
  
}

function plotresult {

  echo "###########################"
  echo "####### PLOT RESULT #######"
  echo "###########################"
  
  cd $_RESULTS
  
    local test=`ls`
    local graph="../../$_GRAPHS"
    local utils="../../$_UTILS"
    local testsinstances="../../$_TESTS_INSTANCES"
    for t in $test
    do
      #for each test folder
      cd $t
      
	#create dir for graph
	mkdir -p $graph/$t
	#TIME
	#for each algorithm result file in test folder
	touch tmp
	echo -e 'Algorithm_Name\tExecution_Time' > tmp
	count=0
	for a in `ls *.txt`
	do
	  #get the first value on text file 
	  time_test=`head -n 1 $a`
	  #if a value of executed time exists
	  if [ ! -z "$time_test" ];
	  then
	    #take first character of all word in file name and create name with them
	    IFS='_' read -a words <<< "$a"
	    name=""
	    for word in "${words[@]}"
	    do
	      firstchar=`echo $word | head -c 1`
	      name="$name$firstchar"
	    done
	    echo -e "$name\t$time_test" >> tmp
	    let count+=1
	  fi
	done
	if [ $count != 0 ];
	then
	  let offset=-5/$count
	  #create output file graph
	  gnuplot <<- EOF
	    set title "`echo $t`"
	    set xlabel "Algorithm Name"
	    set ylabel "Execution Time (s)"
	    set xtics rotate by 90 offset `echo $offset`, 1
	    set ytics out nomirror
	    set term png
	    set output "result.png"
	    set style fill solid border -1
	    # Make the histogram boxes half the width of their slots.
	    set boxwidth 0.5 relative
	    set grid front

	    # Select histogram mode.
	    set style data histograms
	    # Select a row-stacked histogram.
	    set style histogram rowstacked
	    plot "tmp" using 2:xticlabels(1) title 'Algorithm'
		EOF
	fi
	rm -f tmp
	mv result.png $graph/$t/result.png
	
	#MEMORY
	#for each algorithm result file in test folder
	touch tmp
	echo -e 'Algorithm_Name\tMemory_Usage' > tmp
	count=0
	for a in `ls *.mem`
	do
	  #get the first value on text file 
	  time_test=`head -n 1 $a`
	  #if a value of executed time exists
	  if [ ! -z "$time_test" ];
	  then
	    #take first character of all word in file name and create name with them
	    IFS='_' read -a words <<< "$a"
	    name=""
	    for word in "${words[@]}"
	    do
	      firstchar=`echo $word | head -c 1`
	      name="$name$firstchar"
	    done
	    echo -e "$name\t$time_test" >> tmp
	    let count+=1
	  fi
	done
	if [ $count != 0 ];
	then
	  let offset=-5/$count
	  #create output file graph
	  gnuplot <<- EOF
	    set title "`echo $t`"
	    set xlabel "Algorithm Name"
	    set ylabel "Usage Memory (MB)"
	    set xtics rotate by 90 offset `echo $offset`, 1
	    set ytics out nomirror
	    set term png
	    set output "result_memory.png"
	    set style fill solid border -1
	    # Make the histogram boxes half the width of their slots.
	    set boxwidth 0.5 relative
	    set grid front

	    # Select histogram mode.
	    set style data histograms
	    # Select a row-stacked histogram.
	    set style histogram rowstacked
	    plot "tmp" using 2:xticlabels(1) title 'Algorithm'
		EOF
	fi
	rm -f tmp
	mv result_memory.png $graph/$t/result_memory.png
	
	#DISTANCE
	touch tmp
	echo -e 'Algorithm_Name\tDistance_From_Optimal_Result' > tmp
	count=0
	for a in `ls *.bin`
	do
	  #get the first value on text file
	  read_info_file "$_TESTS_INSTANCES/$t/info.txt"
	  $utils/$_BITMATRIX_COMPARATOR "multi_dim_brute_force.bin" $a $_TEST_UPDATES $_TEST_SUBSCRIPTIONS
	  time_test=`head -n 1 "diff.txt"`
	  #if a value of executed time exists
	  if [ ! -z "$time_test" ];
	  then
	    #take first character of all word in file name and create name with them
	    IFS='_' read -a words <<< "$a"
	    name=""
	    for word in "${words[@]}"
	    do
	      firstchar=`echo $word | head -c 1`
	      name="$name$firstchar"
	    done
	    echo -e "$name\t$time_test" >> tmp
	    let count+=1
	  fi
	done
	if [ $count != 0 ];
	then
	  let offset=-5/$count
	  #create output file graph
	  gnuplot <<- EOF
	    set title "`echo $t`"
	    set xlabel "Algorithm Name"
	    set ylabel "Distance"
	    set xtics rotate by 90 offset `echo $offset`, 1
	    set ytics out nomirror
	    set term png
	    set output "result_distance.png"
	    set style fill solid border -1
	    # Make the histogram boxes half the width of their slots.
	    set boxwidth 0.5 relative
	    set grid front

	    # Select histogram mode.
	    set style data histograms
	    # Select a row-stacked histogram.
	    set style histogram rowstacked
	    plot "tmp" using 2:xticlabels(1) title 'Algorithm'
		EOF
	fi
	rm -f tmp
	mv result_distance.png $graph/$t/result_distance.png
      cd ..
    done
  
  cd ..
}

#regex for integer and float
re_integer='^[0-9]+$'
re_float='^[0-9]+([.][0-9]+)?$'

if [ ! -z "$1" ] && [ "$1" == "--help" ];
then

  echo "for build and run all algorithms:
usage: 	./launcher.sh run EXTENTS DIMENSION ALFA ALFA_PARALLEL
  
for only build:
usage:	./launcher.sh build

for configure the run command:
usage:	./launcher.sh configure

for only run, you must pass a parameter that indicates the test type:
usage:	./launcher.sh run <test_type>
example:./launcher.sh run alfa

for clean (all builded objects, Results, Graphs folder, utils and TestsInstances):
usage:	./launcher.sh clean

for clean only algorithm:
usage:	./launcher.sh cleanalgorithms

for clean only _results and _graphs folder:
usage:	./launcher.sh cleanresults

for clean only utils object files:
usage:	./launcher.sh cleanutils

for clean only tests instances folder:
usage:	./launcher.sh cleantestsinstances

for create a new DDM Test Instance:
usage:	./launcher.sh DDMInstanceMaker

for create defaults DDM Tests Instances:
usage:	./launcher.sh DDMDefaultsTests"

elif [ ! -z "$1" ] && [ "$1" == "build" ];
then
 
  #Build all algorithms
  build
 
elif [ ! -z "$1" ] && [ "$1" == "run" ];
then

  #Run all algorithms with the default configuration setted with configure command
  run $2 $3 $4 $5
  
elif [ ! -z "$1" ] && [ "$1" == "clean" ];
then

  #Clean all algorithms build objects and _result and _graphs folder
  clean
  
elif [ ! -z "$1" ] && [ "$1" == "configure" ];
then

  #Configure command set the start extents, max extents to reach, step size to increment start extents, alfa and parallel alfa and max run
  #means the number of each algorithm must be execute with the same settings
  configure

elif [ ! -z "$1" ] && [ "$1" == "cleanresults" ];
then
  
  #Clean _results and _graphs folder
  cleanresults
  
elif [ ! -z "$1" ] && [ "$1" == "cleanalgorithms" ];
then
  
  #Clean algorithms build objects
  cleanalgorithms

elif [ ! -z "$1" ] && [ "$1" == "cleanutils" ];
then
  
  #Clean utils build objects
  cleanutils
 
elif [ ! -z "$1" ] && [ "$1" == "cleantestsinstances" ];
then
  
  #Clean testsinstances folder
  cleantestsinstances 
 
elif [ ! -z "$1" ] && [ "$1" == "DDMInstanceMaker" ];
then
  
  #Create directory for possible future Instances Test
  mkdir -p $_TESTS_INSTANCES
  #View all possible parameters 
  ./$_UTILS/$_DDM_INSTANCE_MAKER
  #Read parameters
  read -p "Insert your parameter string here: " DDMInstanceMakerParameters
  #Executes with parameters
  ./$_UTILS/$_DDM_INSTANCE_MAKER $DDMInstanceMakerParameters
  
elif [ ! -z "$1" ] && [ "$1" == "DDMDefaultsTests" ];
then

  #use script in utils folder to create default Tests Instances
  ./$_UTILS/$_BASH/$_CREATE_INSTANCES_DEFAULT
  
elif [ ! -z "$1" ] && [ "$1" == "plotresult" ];
then

  plotresult
 
else
  
  echo "how to use this program:
./launcher.sh --help"
  
fi
