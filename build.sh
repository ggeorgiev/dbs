#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

CLANGBIN=$BASEDIR/clang/bin
CLANG=clang++

CXXFLAGS="-std=c++11 -stdlib=libc++ -isystem/Developer/SDKs/MacOSX10.7.sdk/usr/include"
CXXFLAGS="$CXXFLAGS -Isrc/err/include"

mkdir -p build
PATH=$CLANGBIN:$PATH $CLANG $CXXFLAGS src/main.cpp -o build/main
