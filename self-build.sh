#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

if [ ! -e dbs ]
then
    ./build.sh
fi

./dbs src/main.dbs build dbs dbs-test || exit 1

build/debug/dbs-test --gtest_filter=-*.PERFORMANCE_* || exit 1

cmp --silent dbs build/debug/dbs || cp dbs dbs.`date +%Y-%m-%dT%H-%M-%S`
cp build/debug/dbs dbs

./dbs src/main.dbs build-script dbs > build/build.sh
cmp --silent build.sh build/build.sh || cp build.sh build.sh.`date +%Y-%m-%dT%H-%M-%S`
cp build/build.sh build.sh

