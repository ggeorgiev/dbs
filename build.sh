#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

CLANGBIN=$BASEDIR/clang/bin
CLANG=clang++

CXXFLAGS="-std=c++11 -stdlib=libc++"
CXXFLAGS="$CXXFLAGS -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk"
CXXFLAGS="$CXXFLAGS -Isrc/err/include"

FILES="src/err/err.cpp"

mkdir -p build
#PATH=$CLANGBIN:$PATH $CLANG $CXXFLAGS src/main.cpp $FILES -o build/main


CXXFLAGS="$CXXFLAGS -Igtest/include"
CXXFLAGS="$CXXFLAGS -Lgtest/lib"

CXXFLAGS="$CXXFLAGS -Iboost/include"
CXXFLAGS="$CXXFLAGS -Lboost/lib"

CXXFLAGS="$CXXFLAGS -Icppformat/include"
CXXFLAGS="$CXXFLAGS -Lcppformat/lib"


FILES="$FILES src/gtest/time_monitor.cpp src/gtest/performance_arbiter.cpp"
FILES="$FILES src/err/gtest/err-utest.cpp"
FILES="$FILES src/err/gtest/err-ptest.cpp"

LIBRARIES="-lgtest -lboost_system -lboost_chrono -lcppformat"

DEFINES=""

PATH=$CLANGBIN:$PATH $CLANG -O3 $CXXFLAGS src/gtest/main.cpp $FILES -o build/gtest-main \
    $DEFINES $LIBRARIES || exit 1

build/gtest-main


