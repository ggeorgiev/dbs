#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

CLANGBIN=$BASEDIR/clang/bin
CLANG=clang++

IWYU=include-what-you-use

CXXFLAGS="-std=c++11 -stdlib=libc++"
CXXFLAGS="$CXXFLAGS -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk"

CXXFLAGS="$CXXFLAGS -Isrc/im/include"
CXXFLAGS="$CXXFLAGS -Isrc/err/include"
CXXFLAGS="$CXXFLAGS -Isrc/dom/include"
CXXFLAGS="$CXXFLAGS -Isrc/parser/include"

FILES="src/err/err.cpp"
FILES="$FILES src/parser/string.cpp"

CXXFLAGS="$CXXFLAGS -isystemgtest/include"
LIBRARIES="$LIBRARIES -Lgtest/lib"

CXXFLAGS="$CXXFLAGS -isystemboost/include"
LIBRARIES="$LIBRARIES -Lboost/lib"

CXXFLAGS="$CXXFLAGS -isystemcppformat/include"
LIBRARIES="$LIBRARIES -Lcppformat/lib"

mkdir -p build
#PATH=$CLANGBIN:$PATH $CLANG $CXXFLAGS src/main.cpp $FILES -o build/main

FILES="$FILES src/gtest/time_monitor.cpp src/gtest/performance_arbiter.cpp"

FILES="$FILES src/im/gtest/initialization_manager-utest.cpp"

FILES="$FILES src/err/gtest/err-utest.cpp"
FILES="$FILES src/err/gtest/err-ptest.cpp"

FILES="$FILES src/dom/gtest/cpp_program-utest.cpp"

FILES="$FILES src/parser/gtest/parser-utest.cpp"
FILES="$FILES src/parser/gtest/stream-utest.cpp"
FILES="$FILES src/parser/gtest/token-utest.cpp"
FILES="$FILES src/parser/gtest/tokenizer-utest.cpp"

LIBRARIES="$LIBRARIES -lgtest -lboost_system -lboost_chrono -lformat"

#DEFINES="-DNDEBUG"
DEFINES=""

if [ 1 == 1 ]
then
    PATH=$CLANGBIN:$PATH $CLANG -O3 $CXXFLAGS src/gtest/main.cpp $FILES -o build/gtest-main \
        $DEFINES $LIBRARIES || exit 1

    build/gtest-main --gtest_filter=-*.PERFORMANCE_* || exit 1

else

    echo > build/iwyu.log
    for FILE in src/parser/gtest/tokenizer-utest.cpp #src/gtest/main.cpp $FILES
    do
        echo include what you using $FILE ...
        PATH=$CLANGBIN:$PATH $IWYU -O3 $CXXFLAGS $FILE $DEFINES 2>&1 | tee -a build/iwyu.log
    done

    #iwyu/bin/fix_includes.py --nosafe_headers < build/iwyu.log
fi


