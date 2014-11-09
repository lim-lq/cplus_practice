#!/bin/bash

counts=0

while [ 1 ]
do
    ./client
    counts=`expr $var + 1`
    if [ $counts -eq 100 ]
    then
        exit 0
    fi
done
