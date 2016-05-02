#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

if [ ! -e dbs ]
then
    cp build/dbs dbs
fi

if [ -e build/dbs ]
then
    mv build/dbs build/dbs_1
fi

./dbs src/main.dbs build dbs dbs-test || exit 1

build/dbs-test --gtest_filter=-*.PERFORMANCE_* || exit 1

if [ -e build/dbs ]
then
    rm dbs
    cp build/dbs dbs
fi
