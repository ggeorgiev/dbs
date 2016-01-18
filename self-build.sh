#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

if [ -e build/self.sh ]
then
    rm build/self.sh
fi

if [ ! -e dbs ]
then
    cp build/main dbs
fi

mkdir -p build
./dbs src/main.dbs | tee build/self.sh
chmod 755 build/self.sh

echo
echo running ...
build/self.sh || exit 1

mv build/main dbs
