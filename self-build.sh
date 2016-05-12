#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

if [ ! -e dbs ]
then
    ./build.sh
fi

./dbs src/main.dbs build dbs dbs-test || exit 1

build/debug/dbs-test --gtest_filter=-*.PERFORMANCE_* || exit 1

cmp --silent dbs build/debug/dbs || cp dbs dbs_`date +%Y-%m-%d:%H:%M:%S`
cp build/debug/dbs dbs


