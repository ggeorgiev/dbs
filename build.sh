#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

CLANGBIN=$BASEDIR/clang/bin
CLANG=clang++

IWYU=include-what-you-use

CXXFLAGS="-std=c++11 -stdlib=libc++"
CXXFLAGS="$CXXFLAGS -isysroot /Applications/Xcode.app/Contents/Developer/Platforms"
CXXFLAGS="$CXXFLAGS/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk"

CXXFLAGS="$CXXFLAGS -Isrc/const/include"
CXXFLAGS="$CXXFLAGS -Isrc/im/include"
CXXFLAGS="$CXXFLAGS -Isrc/err/include"
CXXFLAGS="$CXXFLAGS -Isrc/dom/include"
CXXFLAGS="$CXXFLAGS -Isrc/doim/include"
CXXFLAGS="$CXXFLAGS -Isrc/parser/include"
CXXFLAGS="$CXXFLAGS -Isrc/tpool/include"
CXXFLAGS="$CXXFLAGS -Isrc/tool/include"
CXXFLAGS="$CXXFLAGS -Isrc/task/include"


FILES=""
FILES="$FILES src/err/err.cpp"

FILES="$FILES src/const/constants.cpp"

FILES="$FILES src/doim/manager.cpp"

FILES="$FILES src/dom/manager.cpp"

CXXFLAGS="$CXXFLAGS -isystemgtest/include"
LIBRARIES="$LIBRARIES -Lgtest/lib"

CXXFLAGS="$CXXFLAGS -isystemboost/include"
LIBRARIES="$LIBRARIES -Lboost/lib"

CXXFLAGS="$CXXFLAGS -isystemcppformat/include"
LIBRARIES="$LIBRARIES -Lcppformat/lib"

LIBRARIES="$LIBRARIES -lboost_filesystem -lboost_chrono -lboost_system -lformat"

#DEFINES="-DNDEBUG" && OPTOMIZATION="-O3"
DEFINES="-DDEBUG" && OPTOMIZATION="-O0 -g"

mkdir -p build
PATH=$CLANGBIN:$PATH $CLANG $OPTOMIZATION $CXXFLAGS src/main.cpp $FILES \
    $DEFINES $LIBRARIES -o build/main || exit 1

if [ 1 == 1 ]
then

    CXXFLAGS="$CXXFLAGS -Isrc/gtest/include"

    FILES="$FILES src/gtest/time_monitor.cpp src/gtest/performance_arbiter.cpp"

    FILES="$FILES src/im/gtest/initialization_manager-utest.cpp"

    FILES="$FILES src/err/gtest/err-utest.cpp"
    FILES="$FILES src/err/gtest/err-ptest.cpp"

    FILES="$FILES src/dom/gtest/cxx_program-utest.cpp"

    FILES="$FILES src/doim/gtest/manager-utest.cpp"
    FILES="$FILES src/doim/gtest/fs/fs_directory-utest.cpp"

    FILES="$FILES src/parser/gtest/parser-utest.cpp"
    FILES="$FILES src/parser/gtest/stream-utest.cpp"
    FILES="$FILES src/parser/gtest/token-utest.cpp"
    FILES="$FILES src/parser/gtest/tokenizer-utest.cpp"
    FILES="$FILES src/parser/gtest/cxx/cxx_parser-utest.cpp"

    FILES="$FILES src/tpool/gtest/tpool-utest.cpp"
    FILES="$FILES src/tpool/gtest/priority-utest.cpp"

    FILES="$FILES src/task/gtest/cxx/cxx_file_crc_task-utest.cpp"


    LIBRARIES="$LIBRARIES -lgtest"

    COMPILE_DATABASE=build/compile_commands.json

    echo "[" > $COMPILE_DATABASE
    for FILE in $FILES src/main.cpp src/gtest/main.cpp
    do
        echo '  { "directory": "'$BASEDIR'", ' >> $COMPILE_DATABASE
        echo '    "command": "'$PATH/$CLANG' -c '$OPTOMIZATION' '$CXXFLAGS' ' \
             $FILE' '$DEFINES' -o build/'$FILE'.o", ' >> $COMPILE_DATABASE
        echo '    "file": "'$FILE'" }, ' >> $COMPILE_DATABASE
        echo >> $COMPILE_DATABASE
    done
    echo "]" >> $COMPILE_DATABASE

    if [ 1 == 1 ]
    then
        mkdir -p build/src/gtest
        mkdir -p build/src/err/gtest
        mkdir -p build/src/const/gtest
        mkdir -p build/src/dom/gtest
        mkdir -p build/src/dom/cxx/gtest
        mkdir -p build/src/dom/generic/gtest
        mkdir -p build/src/doim/gtest/fs
        mkdir -p build/src/doim/generic/gtest
        mkdir -p build/src/doim/fs/gtest
        mkdir -p build/src/im/gtest
        mkdir -p build/src/parser/gtest/cxx
        mkdir -p build/src/tpool/gtest
        mkdir -p build/src/task/gtest/cxx

        OBJFILES=
        for FILE in src/gtest/main.cpp $FILES
        do
            echo clang compile $FILE ...

            PATH=$CLANGBIN:$PATH $CLANG -c $OPTOMIZATION $CXXFLAGS \
                $FILE \
                $DEFINES \
                -o build/$FILE.o || exit 1
            OBJFILES="$OBJFILES build/$FILE.o"
        done

        PATH=$CLANGBIN:$PATH $CLANG $OPTOMIZATION $CXXFLAGS \
             $OBJFILES \
             $DEFINES $LIBRARIES \
             -o build/gtest-main || exit 1

        build/gtest-main --gtest_filter=-*.PERFORMANCE_* || exit 1

    elif [ 1 == 0 ]
    then

        echo > build/iwyu.log
        for FILE in src/main.cpp src/gtest/main.cpp $FILES
        do
            echo include what you using $FILE ...
            PATH=$CLANGBIN:$PATH $IWYU $CXXFLAGS $FILE $DEFINES 2>&1 | \
                tee -a build/iwyu.log || exit 1
        done

        iwyu/bin/fix_includes.py --nosafe_headers < build/iwyu.log
    else
        for FILE in src/main.cpp src/gtest/main.cpp $FILES
        do
            echo clang tidy $FILE ...
            PATH=$CLANGBIN:$PATH clang-tidy -checks=*,-google-readability-todo,-cppcoreguidelines-pro-type-vararg,-google-readability-braces-around-statements,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-readability-braces-around-statements -p build $FILE 2>&1 || exit 1
        done
    fi
fi
