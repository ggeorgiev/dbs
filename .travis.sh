#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

# This is a test if all 3rdparty libraries are build. They might not be because we split
# the builds of them in travis, because it takes too long.
if [ ! -e rocksdb ]
then
    exit 0
fi

./build.sh || exit 1
cp build/release/dbs dbs || exit 1
./dbs src/main.dbs coverage dbs dbs-test || exit 1
build/profile/dbs-test --gtest_filter=-*.PERFORMANCE_*  || exit 1

./dbs src/main.dbs generate dbs | grep -v \^\# > build/build.sh
cmp build.sh build/build.sh || exit 1