#!/bin/bash

set -e
# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap 'echo "\"${last_command}\" command filed with exit code $?."' EXIT

defaultInputFileName=$1
defaultOutputFileName=$2

if [ -z "$defaultInputFileName" ]; then
    defaultInputFileName="./input/input2.txt"
fi

if [ -z "$defaultOutputFileName" ]; then
    defaultOutputFileName="./output.txt"
fi

make main

mpirun -np 1 ./bin/finalgame.o "$defaultInputFileName" "$defaultOutputFileName"

