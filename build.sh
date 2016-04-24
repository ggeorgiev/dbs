#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

CLANGBIN=$BASEDIR/clang/bin
CLANG=clang++

IWYU=include-what-you-use

CXXFLAGS="-std=c++11 -stdlib=libc++"
CXXFLAGS="$CXXFLAGS -isysroot /Applications/Xcode.app/Contents/Developer/Platforms"
CXXFLAGS="$CXXFLAGS/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk"

#PLUGINS="-fcolor-diagnostics"
PLUGINS=

PLUGIN=$BASEDIR/clang/plugin/param_check/ParameterNameChecker.so
if [ -e $PLUGIN ]
then
    PLUGINS="$PLUGINS -Xclang -load -Xclang $PLUGIN"
    PLUGINS="$PLUGINS -Xclang -plugin -Xclang check-parameter-names"
fi

CXXFLAGS="$CXXFLAGS -Isrc"

FILES=""
FILES="$FILES src/log/log.cpp"
FILES="$FILES src/err/err.cpp"
FILES="$FILES src/const/constants.cpp"
FILES="$FILES src/db/database.cpp"
FILES="$FILES src/doim/cxx/cxx_file.cpp"
FILES="$FILES src/doim/cxx/cxx_header.cpp"
FILES="$FILES src/doim/cxx/cxx_include_directory.cpp"
FILES="$FILES src/doim/cxx/cxx_object_file.cpp"
FILES="$FILES src/doim/cxx/cxx_program.cpp"
FILES="$FILES src/doim/cxx/cxx_static_library.cpp"
FILES="$FILES src/doim/fs/fs_directory.cpp"
FILES="$FILES src/doim/fs/fs_file.cpp"
FILES="$FILES src/doim/generic/object.cpp"
FILES="$FILES src/doim/sys/command.cpp"
FILES="$FILES src/doim/tag/tag.cpp"
FILES="$FILES src/doim/manager.cpp"
FILES="$FILES src/dom/manager.cpp"
FILES="$FILES src/engine/cxx_engine.cpp"
FILES="$FILES src/option/verbose.cpp"
FILES="$FILES src/task/cxx/cxx_crc_task_mixin.cpp"
FILES="$FILES src/task/cxx/cxx_file_crc_task.cpp"
FILES="$FILES src/task/cxx/cxx_header_crc_task.cpp"
FILES="$FILES src/task/cxx/cxx_object_file_crc_task.cpp"
FILES="$FILES src/task/cxx/cxx_program_crc_task.cpp"
FILES="$FILES src/task/db/db_put_task.cpp"
FILES="$FILES src/task/sys/ensure_directory_task.cpp"
FILES="$FILES src/task/sys/execute_command_task.cpp"
FILES="$FILES src/task/manager.cpp"
FILES="$FILES src/task/tpool.cpp"
FILES="$FILES src/tool/cxx_compiler.cpp"
FILES="$FILES src/tpool/task.cpp"
FILES="$FILES src/tpool/task_callback.cpp"
FILES="$FILES src/tpool/task_group.cpp"
FILES="$FILES src/tpool/task_sequence.cpp"
FILES="$FILES src/tpool/tpool.cpp"

CXXFLAGS="$CXXFLAGS -isystemspdlog/include"

CXXFLAGS="$CXXFLAGS -isystemgtest/include"
LIBRARIES="$LIBRARIES -Lgtest/lib"

CXXFLAGS="$CXXFLAGS -isystemboost/include"
LIBRARIES="$LIBRARIES -Lboost/lib"

CXXFLAGS="$CXXFLAGS -isystemcppformat/include"
LIBRARIES="$LIBRARIES -Lcppformat/lib"

CXXFLAGS="$CXXFLAGS -isystemrocksdb/include"
LIBRARIES="$LIBRARIES -Lrocksdb/lib"

LIBRARIES="$LIBRARIES -lboost_filesystem -lboost_chrono -lboost_system -lboost_thread"
LIBRARIES="$LIBRARIES -lformat"
LIBRARIES="$LIBRARIES -lrocksdb -lz -lbz2"

DEFINES=" -DNDEBUG" && OPTOMIZATION="-O3"
#DEFINES=" -DDEBUG" && OPTOMIZATION="-O0 -g"


if [ 1 == 1 ]
then
    mkdir -p build
    PATH=$CLANGBIN:$PATH $CLANG $OPTOMIZATION $CXXFLAGS src/main.cpp $FILES \
        $DEFINES $LIBRARIES -o build/dbs && cp build/dbs dbs || exit 1
fi

if [ 1 == 0 ]
then

    CXXFLAGS="$CXXFLAGS -Isrc/gtest/include"

    FILES="$FILES src/gtest/time_monitor.cpp src/gtest/performance_arbiter.cpp"

    FILES="$FILES src/im/gtest/initialization_manager-utest.cpp"

    FILES="$FILES src/log/gtest/log-utest.cpp"

    FILES="$FILES src/err/gtest/err-utest.cpp"
    FILES="$FILES src/err/gtest/err-ptest.cpp"
    FILES="$FILES src/err/gtest/macro-utest.cpp"

    FILES="$FILES src/db/gtest/database-utest.cpp"

    FILES="$FILES src/dom/gtest/cxx_library-utest.cpp"
    FILES="$FILES src/dom/gtest/cxx_program-utest.cpp"

    FILES="$FILES src/doim/gtest/manager-utest.cpp"
    FILES="$FILES src/doim/gtest/fs/fs_directory-utest.cpp"
    FILES="$FILES src/doim/gtest/fs/fs_file-ptest.cpp"

    FILES="$FILES src/parser/gtest/parser-utest.cpp"
    FILES="$FILES src/parser/gtest/stream-utest.cpp"
    FILES="$FILES src/parser/gtest/token-utest.cpp"
    FILES="$FILES src/parser/gtest/tokenizer-utest.cpp"
    FILES="$FILES src/parser/gtest/cxx/cxx_parser-utest.cpp"

    FILES="$FILES src/tpool/gtest/tpool-utest.cpp"
    FILES="$FILES src/tpool/gtest/priority-utest.cpp"

    FILES="$FILES src/task/gtest/cxx/cxx_file_crc_task-utest.cpp"
    FILES="$FILES src/task/gtest/cxx/cxx_header_crc_task-utest.cpp"


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
        mkdir -p build/src/log/gtest
        mkdir -p build/src/err/gtest
        mkdir -p build/src/const/gtest
        mkdir -p build/src/db/gtest
        mkdir -p build/src/dom/gtest
        mkdir -p build/src/dom/cxx/gtest
        mkdir -p build/src/dom/generic/gtest
        mkdir -p build/src/doim/gtest/fs
        mkdir -p build/src/doim/generic/gtest
        mkdir -p build/src/doim/fs/gtest
        mkdir -p build/src/im/gtest
        mkdir -p build/src/parser/gtest/cxx
        mkdir -p build/src/tpool/gtest
        mkdir -p build/src/task/cxx
        mkdir -p build/src/task/gtest/cxx

        OBJFILES=
        for FILE in src/gtest/main.cpp $FILES
        do
            echo clang compile $FILE ...

            PATH=$CLANGBIN:$PATH $CLANG $PLUGINS -c $OPTOMIZATION $CXXFLAGS \
                $DEFINES \
                $FILE \
                -o build/$FILE.o || exit 1
            OBJFILES="$OBJFILES build/$FILE.o"
        done

        PATH=$CLANGBIN:$PATH $CLANG $OPTOMIZATION $CXXFLAGS \
             $OBJFILES \
             $DEFINES $LIBRARIES \
             -o build/dbs-test || exit 1

        build/dbs-test --gtest_filter=-*.PERFORMANCE_* || exit 1

    elif [ 1 == 1 ]
    then

        echo > build/iwyu.log
        for FILE in src/main.cpp src/gtest/main.cpp $FILES
        do
            echo include what you using $FILE ...
            PATH=$CLANGBIN:$PATH $IWYU $CXXFLAGS $FILE $DEFINES 2>&1 | \
                tee -a build/iwyu.log || exit 1
        done

        iwyu/bin/fix_includes.py --noblank_lines --nosafe_headers < build/iwyu.log
    else
        for FILE in src/main.cpp src/gtest/main.cpp $FILES
        do
            echo clang tidy $FILE ...
            PATH=$CLANGBIN:$PATH clang-tidy -checks=*,-llvm-include-order,-google-readability-todo,-cppcoreguidelines-pro-type-vararg,-google-readability-braces-around-statements,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-readability-braces-around-statements -p build $FILE 2>&1 || exit 1
            echo
            #echo clang check $FILE ...
            #PATH=$CLANGBIN:$PATH clang-check -analyze -extra-arg -Xclang -extra-arg -analyzer-output=text -p build $FILE 2>&1 || exit 1
            #echo
        done
    fi
fi
