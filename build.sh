#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

CLANGBIN=$BASEDIR/clang/bin
CLANG=clang++

IWYU=include-what-you-use

CXXFLAGS="-std=c++11 -stdlib=libc++"
CXXFLAGS="$CXXFLAGS -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk"

CXXFLAGS="$CXXFLAGS -Isrc/const/include"
CXXFLAGS="$CXXFLAGS -Isrc/im/include"
CXXFLAGS="$CXXFLAGS -Isrc/err/include"
CXXFLAGS="$CXXFLAGS -Isrc/dom/include"
CXXFLAGS="$CXXFLAGS -Isrc/parser/include"

FILES="src/err/err.cpp"

FILES="$FILES src/const/constants.cpp"

FILES="$FILES src/dom/fs/fs_manager.cpp"

CXXFLAGS="$CXXFLAGS -isystemgtest/include"
LIBRARIES="$LIBRARIES -Lgtest/lib"

CXXFLAGS="$CXXFLAGS -isystemboost/include"
LIBRARIES="$LIBRARIES -Lboost/lib"

CXXFLAGS="$CXXFLAGS -isystemcppformat/include"
LIBRARIES="$LIBRARIES -Lcppformat/lib"

LIBRARIES="$LIBRARIES -lboost_system -lboost_chrono -lformat"

#DEFINES="-DNDEBUG" && OPTOMIZATION="-O3"
DEFINES="-DDEBUG" && OPTOMIZATION="-O0"

mkdir -p build
PATH=$CLANGBIN:$PATH $CLANG $OPTOMIZATION $CXXFLAGS \
    src/main.cpp $FILES \
    $DEFINES $LIBRARIES \
    -o build/main  || exit 1

if [ 1 == 0 ]
then

    FILES="$FILES src/gtest/time_monitor.cpp src/gtest/performance_arbiter.cpp"

    FILES="$FILES src/im/gtest/initialization_manager-utest.cpp"

    FILES="$FILES src/err/gtest/err-utest.cpp"
    FILES="$FILES src/err/gtest/err-ptest.cpp"

    FILES="$FILES src/dom/gtest/cpp_program-utest.cpp"
    FILES="$FILES src/dom/gtest/fs_manager-utest.cpp"
    FILES="$FILES src/dom/gtest/fs_directory-utest.cpp"

    FILES="$FILES src/parser/gtest/parser-utest.cpp"
    FILES="$FILES src/parser/gtest/stream-utest.cpp"
    FILES="$FILES src/parser/gtest/token-utest.cpp"
    FILES="$FILES src/parser/gtest/tokenizer-utest.cpp"

    LIBRARIES="$LIBRARIES -lgtest"

    if [ 1 == 1 ]
    then
        PATH=$CLANGBIN:$PATH $CLANG $OPTOMIZATION $CXXFLAGS \
            src/gtest/main.cpp $FILES \
            -o build/gtest-main \
            $DEFINES $LIBRARIES || exit 1

        build/gtest-main --gtest_filter=-*.PERFORMANCE_* || exit 1

    else

        echo > build/iwyu.log
        for FILE in src/main.cpp src/gtest/main.cpp $FILES
        do
            echo include what you using $FILE ...
            PATH=$CLANGBIN:$PATH $IWYU $CXXFLAGS $FILE $DEFINES 2>&1 | \
                tee -a build/iwyu.log || exit 1
        done

        iwyu/bin/fix_includes.py --nosafe_headers < build/iwyu.log
    fi

fi
