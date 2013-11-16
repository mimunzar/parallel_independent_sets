#!/bin/bash


VERTICES_MIN=100
VERTICES_MAX=400
VERTICES_STEP=5

EDGES_MIN=0.4
EDGES_MAX=0.7
EDGES_STEP=0.1

GENERATOR="./gen_gml.py"

export LC_NUMERIC=en_US.UTF-8

echo "Cleaning all *.gml files from current directory"
for f in `find . -maxdepth 1 -name '*.gml'`
do
    rm -i $f
done

for ratio in `seq $EDGES_MIN $EDGES_STEP $EDGES_MAX`
do
    echo "Generating graphs for $ratio"
    rm -rf $ratio
    for vertices in `seq $VERTICES_MIN $VERTICES_STEP $VERTICES_MAX`
    do
        $GENERATOR -v $vertices -e $ratio
        ret=$?
        if [ $ret -ne 0 ]; then
            echo "Generator returned non zero return code. Exiting..."
            exit
        fi
    done
    mkdir $ratio
    mv *.gml $ratio
done
