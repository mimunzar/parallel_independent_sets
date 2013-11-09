#!/bin/bash

BIN="./build/gal"
SEQ_OUT=`mktemp -t XXXXXgal`
PAR_OUT=`mktemp -t XXXXXgal`
SEQ_ERR=`mktemp -t XXXXXgal`
PAR_ERR=`mktemp -t XXXXXgal`

Clean()
{
    rm -f $SEQ_OUT
    rm -f $PAR_OUT
    rm -f $SEQ_ERR
    rm -f $PAR_ERR
    echo
    exit
}

ParseRunningTime()
{
    cat $1 | sed -E 's/Running time \[s\]://' | tr -d '\n' | tr -d ' '
}

if [ $# -ne 1 ]; then
    echo "You have to pass directory with graphs as argument" >&2
    exit
fi

GRAPHS_DIR="$1"


UnifyOutputs()
{
    perl -ni -e 'print unless $. == 1' $1
    sort $1 -o $1
}


trap Clean SIGHUP SIGTERM SIGINT

LC_NUMERIC="en_US.UTF-8"

printf "%15s %15s %15s %15s\n" "Graph" "Sequence" "Parallel" "Speedup"
for g in `find $GRAPHS_DIR -type f -name '*.gml' -maxdepth 1`
do
    bname=`basename $g`
    printf "%15s" $bname
    $BIN $g 2> $SEQ_ERR > $SEQ_OUT
    seqTime=`ParseRunningTime $SEQ_ERR`
    printf "%16.3f" $seqTime
    $BIN -p $g 2> $PAR_ERR > $PAR_OUT
    parTime=`ParseRunningTime $PAR_ERR`
    printf "%16.3f" $parTime
    speedup=`bc -l <<< "$seqTime / $parTime"`
    printf "%16.3f\n" $speedup

    UnifyOutputs $SEQ_OUT
    UnifyOutputs $PAR_OUT
    if [[ ! -z "`diff $SEQ_OUT $PAR_OUT`" ]]; then
        echo "!!!! OUTPUT OF SEQUENCE AND PARALLEL ALGORITHM DIFERSS!!!!" >&2
        echo " -- DIFF:" >&2
        diff $SEQ_OUT $PAR_OUT >&2
        #echo " -- SEQUENCE:" >&2
        #cat $SEQ_OUT >&2
        #echo " -- PARALLEL:" >&2
        #cat $PAR_OUT >&2
    fi
done

Clean

