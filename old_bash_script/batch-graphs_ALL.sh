#!/bin/bash

./batch-speedup.sh

cd _graphs

for f in *.gp
do
	echo "Processing $f file..."
	gnuplot $f
done

cd ..
