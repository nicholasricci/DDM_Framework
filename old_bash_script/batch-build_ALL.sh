#!/bin/bash

mkdir -p _bin/
#
cd interval_tree ; make
cp interval_tree brute_force ../_bin/
make clean
make openmp
cp interval_tree ../_bin/interval_tree_mp
cp brute_force ../_bin/brute_force_mp
cd ..
#
cd sort_matching_standard ; make
cp bin/sort_matching_standard.amd64.Release ../_bin/
cd ..
#
cd utils ; make ; cd ..
