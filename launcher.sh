#!/bin/bash

echo "Launcher starts!"

echo "Move to utils and clean"
cd utils
make clean
echo "Make makefile"
make
