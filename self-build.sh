#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

rm build/self.sh

build/main src/main.dbs | tee build/self.sh
chmod 755 build/self.sh

build/self.sh
