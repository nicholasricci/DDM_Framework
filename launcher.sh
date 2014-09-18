#!/bin/bash

echo "Launcher starts!"

echo "Move to utils and clean"
cd launcher
make clean
echo "Make makefile"
make
