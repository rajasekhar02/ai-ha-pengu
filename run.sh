#!/bin/bash


defaultInputFileName=$1
defaultOutputFileName=$2

if [ -z "$defaultInputFileName"]; then
    defaultInputFileName="./testcase.txt"
fi

if [ -z "$defaultOutputFileName" ]; then
    defaultOutputFileName="./output.txt"
fi

node solution.js "$defaultInputFileName" "$defaultOutputFileName"