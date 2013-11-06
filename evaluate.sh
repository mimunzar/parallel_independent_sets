#!/bin/bash

GRAPHS_DIR="./graphs"
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
    exit
}

ParseRunningTime()
{
    cat $1 | sed -E 's/Running time \[s\]://' | tr '\n' '\t'   
}

trap Clean SIGHUP SIGTERM SIGINT


for g in `find $GRAPHS_DIR -type f -name '*.gml'`
do
    echo -en "$g\t"
    $BIN $g 2> $SEQ_ERR | tail -n +2 | sort > $SEQ_OUT
    $BIN -p $g 2> $PAR_ERR | tail -n +2 | sort > $PAR_OUT
    ParseRunningTime $SEQ_ERR
    ParseRunningTime $PAR_ERR
    echo
    if [[ ! -z "`diff $SEQ_OUT $PAR_OUT`" ]]; then
        echo "!!!! OUTPUT OF SEQUENCE AND PARALLEL ALGORITHM DIFERSS!!!!" >&2
        echo " -- DIFF:" >&2
        diff $SEQ_OUT $PAR_OUT >&2
        echo " -- SEQUENCE:" >&2
        cat $SEQ_OUT >&2
        echo " -- PARALLE:" >&2
        cat $PAR_OUT >&2
    fi
done

Clean

