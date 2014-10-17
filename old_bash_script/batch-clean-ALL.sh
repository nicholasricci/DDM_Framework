#!/bin/bash

cd interval_tree ; make clean ; cd ..
cd sort_matching_standard ; make clean ; cd ..
cd utils ; make clean ; cd ..

rm -f _graphs/speed*.txt
rm -f _graphs/*.ps
rm -f _graphs/*.eps
rm -f _results/*.txt
rm -fr _bin/
