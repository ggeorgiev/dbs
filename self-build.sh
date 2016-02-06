#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

if [ -e build/self.sh ]
then
    rm build/self.sh
fi

if [ ! -e dbs ]
then
    cp build/dbs dbs
fi

if [ -e build/dbs ]
then
    mv build/dbs build/dbs_1
fi

mkdir -p build
(./dbs src/main.dbs dbs dbs-test | tee build/self.sh) || exit 1
chmod 755 build/self.sh

echo
echo running ...
build/self.sh || exit 1

build/dbs-test --gtest_filter=-*.PERFORMANCE_* || exit 1

if [ -e build/dbs ]
then
    rm dbs
    cp build/dbs dbs
fi
