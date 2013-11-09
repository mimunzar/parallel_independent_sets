#!/bin/bash

mkdir -p results
for d in ../graphs/0.*
do
    bname=`basename $d`
    echo "================================== $bname ======================================"
    ./evaluate.sh $d | tee results/$bname
done

