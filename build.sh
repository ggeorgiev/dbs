#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

IWYU=include-what-you-use

CXXFLAGS="-std=c++14 -stdlib=libc++"
CXXFLAGS="$CXXFLAGS -isysroot /Applications/Xcode.app/Contents/Developer/Platforms"
CXXFLAGS="$CXXFLAGS/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk"

CXXFLAGS="$CXXFLAGS -Isrc -Isrc/system"

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
FILES="$FILES src/doim/db/db_key.cpp"
FILES="$FILES src/doim/fs/fs_binary.cpp"
FILES="$FILES src/doim/fs/fs_directory.cpp"
FILES="$FILES src/doim/fs/fs_file.cpp"
FILES="$FILES src/doim/generic/object.cpp"
FILES="$FILES src/doim/sys/executable.cpp"
FILES="$FILES src/doim/sys/argument.cpp"
FILES="$FILES src/doim/sys/command.cpp"
FILES="$FILES src/doim/tag/tag.cpp"
FILES="$FILES src/dom/cxx/cxx_library.cpp"
FILES="$FILES src/dom/cxx/cxx_program.cpp"
FILES="$FILES src/dom/manager.cpp"
FILES="$FILES src/parser/cxx/cxx_parser.cpp"
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
FILES="$FILES src/task/sys/parse_stdout_task.cpp"
FILES="$FILES src/task/manager.cpp"
FILES="$FILES src/task/tpool.cpp"
FILES="$FILES src/tool/cxx_clang_format.cpp"
FILES="$FILES src/tool/cxx_compiler.cpp"
FILES="$FILES src/tool/cxx_iwyu.cpp"
FILES="$FILES src/tpool/task.cpp"
FILES="$FILES src/tpool/task_callback.cpp"
FILES="$FILES src/tpool/task_group.cpp"
FILES="$FILES src/tpool/tpool.cpp"

CXXFLAGS="$CXXFLAGS -isystemspdlog/include"

CXXFLAGS="$CXXFLAGS -isystemgtest/include"
LIBRARIES="$LIBRARIES -Lgtest/lib"

CXXFLAGS="$CXXFLAGS -isystemboost/include"
LIBRARIES="$LIBRARIES -Lboost/lib"

CXXFLAGS="$CXXFLAGS -isystemfmt/include"
LIBRARIES="$LIBRARIES -Lfmt/lib"

CXXFLAGS="$CXXFLAGS -isystemrocksdb/include"
LIBRARIES="$LIBRARIES -Lrocksdb/lib"

LIBRARIES="$LIBRARIES -lboost_filesystem -lboost_chrono -lboost_system -lboost_thread"
LIBRARIES="$LIBRARIES -lfmt"
LIBRARIES="$LIBRARIES -lrocksdb -lz -lbz2"

DEFINES=" -DNDEBUG" && OPTOMIZATION="-O3"
#DEFINES=" -DDEBUG" && OPTOMIZATION="-O0 -g"


if [ 1 == 1 ]
then
    mkdir -p build
    clang++ $OPTOMIZATION $CXXFLAGS src/main.cpp $FILES \
        $DEFINES $LIBRARIES -o build/dbs && cp build/dbs dbs || exit 1
fi
