# 22:05:58.490 I > Load dbs file: src/main.dbs
# 22:05:58.501 I > Evaluating targets for build-script
if [ ! -e build/ ]; then mkdir build/; fi
if [ ! -e build/release/ ]; then mkdir build/release/; fi
if [ ! -e build/release/src/ ]; then mkdir build/release/src/; fi
if [ ! -e build/release/src/tpool/ ]; then mkdir build/release/src/tpool/; fi
clang++ -I src/ -O3 -c src/tpool/tpool.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/tpool/tpool.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/tpool/task_callback.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/tpool/task_callback.cpp.o \
    -std=c++14
clang++ -I src/ -O3 -c src/tpool/task.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/tpool/task.cpp.o -std=c++14
if [ ! -e build/release/src/const/ ]; then mkdir build/release/src/const/; fi
clang++ -I src/ -O3 -c src/const/constants.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem src/system/ -o build/release/src/const/constants.cpp.o -std=c++14
if [ ! -e build/release/src/dom/ ]; then mkdir build/release/src/dom/; fi
if [ ! -e build/release/src/dom/cxx/ ]; then mkdir build/release/src/dom/cxx/; fi
clang++ -I src/ -O3 -c src/dom/cxx/cxx_library.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/dom/cxx/cxx_library.cpp.o \
    -std=c++14
clang++ -I src/ -O3 -c src/dom/manager.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/dom/manager.cpp.o -std=c++14
if [ ! -e build/release/src/doim/ ]; then mkdir build/release/src/doim/; fi
if [ ! -e build/release/src/doim/cxx/ ]; then mkdir build/release/src/doim/cxx/; fi
clang++ -I src/ -O3 -c src/doim/cxx/cxx_header.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/cxx/cxx_header.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/doim/cxx/cxx_include_directory.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/cxx/cxx_include_directory.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/doim/cxx/cxx_file.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/cxx/cxx_file.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/dom/cxx/cxx_program.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/dom/cxx/cxx_program.cpp.o \
    -std=c++14
clang++ -I src/ -O3 -c src/doim/cxx/cxx_program.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/cxx/cxx_program.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/tpool/task_group.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/tpool/task_group.cpp.o -std=c++14
if [ ! -e build/release/src/doim/generic/ ]; then mkdir build/release/src/doim/generic/; fi
clang++ -I src/ -O3 -c src/doim/generic/object.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/generic/object.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/doim/cxx/cxx_object_file.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/cxx/cxx_object_file.cpp.o -std=c++14
if [ ! -e build/release/src/doim/sys/ ]; then mkdir build/release/src/doim/sys/; fi
clang++ -I src/ -O3 -c src/doim/sys/command.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/sys/command.cpp.o -std=c++14
if [ ! -e build/release/src/log/ ]; then mkdir build/release/src/log/; fi
clang++ -I src/ -O3 -c src/log/log.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/log/log.cpp.o -std=c++14
if [ ! -e build/release/src/engine/ ]; then mkdir build/release/src/engine/; fi
clang++ -I src/ -O3 -c src/engine/cxx_engine.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/engine/cxx_engine.cpp.o -std=c++14
if [ ! -e build/release/src/task/ ]; then mkdir build/release/src/task/; fi
if [ ! -e build/release/src/task/cxx/ ]; then mkdir build/release/src/task/cxx/; fi
clang++ -I src/ -O3 -c src/task/cxx/cxx_file_crc_task.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/cxx/cxx_file_crc_task.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/doim/sys/executable.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/sys/executable.cpp.o -std=c++14
if [ ! -e build/release/src/doim/tag/ ]; then mkdir build/release/src/doim/tag/; fi
clang++ -I src/ -O3 -c src/doim/tag/tag.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/tag/tag.cpp.o -std=c++14
if [ ! -e build/release/src/err/ ]; then mkdir build/release/src/err/; fi
clang++ -I src/ -O3 -c src/err/err.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/err/err.cpp.o -std=c++14
if [ ! -e build/release/src/doim/db/ ]; then mkdir build/release/src/doim/db/; fi
clang++ -I src/ -O3 -c src/doim/db/db_key.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/db/db_key.cpp.o -std=c++14
if [ ! -e build/release/src/doim/fs/ ]; then mkdir build/release/src/doim/fs/; fi
clang++ -I src/ -O3 -c src/doim/fs/fs_file.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/fs/fs_file.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/doim/fs/fs_directory.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/fs/fs_directory.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/task/cxx/cxx_header_crc_task.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/cxx/cxx_header_crc_task.cpp.o -std=c++14
if [ ! -e build/release/src/task/sys/ ]; then mkdir build/release/src/task/sys/; fi
clang++ -I src/ -O3 -c src/task/sys/ensure_directory_task.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/sys/ensure_directory_task.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/main.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/main.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/task/cxx/cxx_crc_task_mixin.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/cxx/cxx_crc_task_mixin.cpp.o -std=c++14
if [ ! -e build/release/src/parser/ ]; then mkdir build/release/src/parser/; fi
if [ ! -e build/release/src/parser/cxx/ ]; then mkdir build/release/src/parser/cxx/; fi
clang++ -I src/ -O3 -c src/parser/cxx/cxx_parser.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/parser/cxx/cxx_parser.cpp.o \
    -std=c++14
clang++ -I src/ -O3 -c src/task/cxx/cxx_program_crc_task.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/cxx/cxx_program_crc_task.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/task/sys/execute_command_task.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/sys/execute_command_task.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/doim/sys/argument.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/sys/argument.cpp.o -std=c++14
if [ ! -e build/release/src/db/ ]; then mkdir build/release/src/db/; fi
clang++ -I src/ -O3 -c src/db/database.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/db/database.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/task/sys/parse_stdout_task.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/sys/parse_stdout_task.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/task/tpool.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/tpool.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/task/manager.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/manager.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/doim/fs/fs_binary.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/fs/fs_binary.cpp.o -std=c++14
if [ ! -e build/release/src/option/ ]; then mkdir build/release/src/option/; fi
clang++ -I src/ -O3 -c src/option/verbose.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/option/verbose.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/task/cxx/cxx_object_file_crc_task.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/cxx/cxx_object_file_crc_task.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/doim/cxx/cxx_static_library.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem src/system/ \
    -o build/release/src/doim/cxx/cxx_static_library.cpp.o -std=c++14
if [ ! -e build/release/src/task/db/ ]; then mkdir build/release/src/task/db/; fi
clang++ -I src/ -O3 -c src/task/db/db_put_task.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/db/db_put_task.cpp.o -std=c++14
if [ ! -e build/release/src/tool/ ]; then mkdir build/release/src/tool/; fi
clang++ -I src/ -O3 -c src/tool/cxx_clang_format.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/tool/cxx_clang_format.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/tool/cxx_compiler.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/tool/cxx_compiler.cpp.o -std=c++14
clang++ -I src/ -O3 -c src/tool/cxx_iwyu.cpp \
    -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk \
    -isystem ../../../../Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/tool/cxx_iwyu.cpp.o -std=c++14
clang++ -L boost/lib/ -L fmt/lib/ -L rocksdb/lib/ -L src/system/ \
    -lboost_filesystem -lboost_system -lboost_thread -lbz2 -lfmt -lrocksdb -lz \
    -o build/release/dbs -stdlib=libc++ build/release/src/const/constants.cpp.o \
    build/release/src/db/database.cpp.o \
    build/release/src/doim/cxx/cxx_file.cpp.o \
    build/release/src/doim/cxx/cxx_header.cpp.o \
    build/release/src/doim/cxx/cxx_include_directory.cpp.o \
    build/release/src/doim/cxx/cxx_object_file.cpp.o \
    build/release/src/doim/cxx/cxx_program.cpp.o \
    build/release/src/doim/cxx/cxx_static_library.cpp.o \
    build/release/src/doim/db/db_key.cpp.o \
    build/release/src/doim/fs/fs_binary.cpp.o \
    build/release/src/doim/fs/fs_directory.cpp.o \
    build/release/src/doim/fs/fs_file.cpp.o \
    build/release/src/doim/generic/object.cpp.o \
    build/release/src/doim/sys/argument.cpp.o \
    build/release/src/doim/sys/command.cpp.o \
    build/release/src/doim/sys/executable.cpp.o \
    build/release/src/doim/tag/tag.cpp.o \
    build/release/src/dom/cxx/cxx_library.cpp.o \
    build/release/src/dom/cxx/cxx_program.cpp.o \
    build/release/src/dom/manager.cpp.o \
    build/release/src/engine/cxx_engine.cpp.o build/release/src/err/err.cpp.o \
    build/release/src/log/log.cpp.o build/release/src/main.cpp.o \
    build/release/src/option/verbose.cpp.o \
    build/release/src/parser/cxx/cxx_parser.cpp.o \
    build/release/src/task/cxx/cxx_crc_task_mixin.cpp.o \
    build/release/src/task/cxx/cxx_file_crc_task.cpp.o \
    build/release/src/task/cxx/cxx_header_crc_task.cpp.o \
    build/release/src/task/cxx/cxx_object_file_crc_task.cpp.o \
    build/release/src/task/cxx/cxx_program_crc_task.cpp.o \
    build/release/src/task/db/db_put_task.cpp.o \
    build/release/src/task/manager.cpp.o \
    build/release/src/task/sys/ensure_directory_task.cpp.o \
    build/release/src/task/sys/execute_command_task.cpp.o \
    build/release/src/task/sys/parse_stdout_task.cpp.o \
    build/release/src/task/tpool.cpp.o \
    build/release/src/tool/cxx_clang_format.cpp.o \
    build/release/src/tool/cxx_compiler.cpp.o \
    build/release/src/tool/cxx_iwyu.cpp.o build/release/src/tpool/task.cpp.o \
    build/release/src/tpool/task_callback.cpp.o \
    build/release/src/tpool/task_group.cpp.o build/release/src/tpool/tpool.cpp.o
