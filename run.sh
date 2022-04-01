#!/bin/bash

set -e
# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap 'echo "\"${last_command}\" command filed with exit code $?."' EXIT

defaultInputFileName=$1
defaultOutputFileName=$2

if [ -z "$defaultInputFileName" ]; then
    defaultInputFileName="./input/input.txt"
fi

if [ -z "$defaultOutputFileName" ]; then
    defaultOutputFileName="./output.txt"
fi

max_depth = 4

cutoff_depth = 3

max_work = 7

max_children = 7

make main

# "$defaultOutputFileName" 

mpirun -np 3 ./bin/finalgame.o "$defaultInputFileName" "$max_depth" "$cutoff_depth" "$max_work" "$max_children"

