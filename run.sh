#!/bin/bash


defaultInputFileName=$1
defaultOutputFileName=$2

if [ -z "$defaultInputFileName"]; then
    defaultInputFileName="./testcase.txt"
fi

if [ -z "$defaultOutputFileName" ]; then
    defaultOutputFileName="./output.txt"
fi

"C:\Program Files\nodejs\node.exe" solution.js "$defaultInputFileName" "$defaultOutputFileName"