#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

find src -name *.cpp -o -name *.h -o -name *.hpp | xargs clang/bin/clang-format -sort-includes -i
