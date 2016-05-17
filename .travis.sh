#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

./build.sh || exit 1
cp build/release/dbs dbs || exit 1
./dbs src/main.dbs coverage dbs dbs-test || exit 1
build/profile/dbs-test --gtest_filter=-*.PERFORMANCE_*  || exit 1

./dbs src/main.dbs build-script dbs > build/build.sh
cmp build.sh build/build.sh || exit 1