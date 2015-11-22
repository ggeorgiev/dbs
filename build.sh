#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

CLANGBIN=$BASEDIR/clang/bin
CLANG=clang++

CXXFLAGS="-std=c++11 -stdlib=libc++ -isystem/Developer/SDKs/MacOSX10.7.sdk/usr/include"
CXXFLAGS="$CXXFLAGS -Isrc/err/include"

mkdir -p build
PATH=$CLANGBIN:$PATH $CLANG $CXXFLAGS src/main.cpp -o build/main


CXXFLAGS="$CXXFLAGS -Igtest/include"
CXXFLAGS="$CXXFLAGS -Lgtest/lib"

FILES="src/gtest/main.cpp src/err/gtest/err.cpp"

PATH=$CLANGBIN:$PATH $CLANG $CXXFLAGS $FILES -o build/gtest-main -lgtest

build/gtest-main
