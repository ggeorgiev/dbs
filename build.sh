if [ ! -e build/ ]; then mkdir build/; fi
if [ ! -e build/release/ ]; then mkdir build/release/; fi
if [ ! -e build/release/src/ ]; then mkdir build/release/src/; fi
if [ ! -e build/release/src/rpc/ ]; then mkdir build/release/src/rpc/; fi
echo Compile src/rpc/rpc.proto
pushd src/ > /dev/null && \
../protobuf/bin/protoc --cpp_out=. rpc/rpc.proto && \
popd > /dev/null &
wait
if [ ! -e build/release/src/const/ ]; then mkdir build/release/src/const/; fi
echo Compile src/const/constants.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/const/constants.cpp -isystem src/system/ \
    -o build/release/src/const/constants.cpp.o -std=c++14 &
if [ ! -e build/release/src/db/ ]; then mkdir build/release/src/db/; fi
echo Compile src/db/database.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/db/database.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/db/database.cpp.o -std=c++14 &
if [ ! -e build/release/src/doim/ ]; then mkdir build/release/src/doim/; fi
if [ ! -e build/release/src/doim/cxx/ ]; then mkdir build/release/src/doim/cxx/; fi
echo Compile src/doim/cxx/cxx_file.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/cxx/cxx_file.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/cxx/cxx_file.cpp.o -std=c++14 &
echo Compile src/doim/cxx/cxx_header.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/cxx/cxx_header.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/cxx/cxx_header.cpp.o \
    -std=c++14 &
echo Compile src/doim/cxx/cxx_include_directory.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/cxx/cxx_include_directory.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ \
    -o build/release/src/doim/cxx/cxx_include_directory.cpp.o -std=c++14 &
echo Compile src/doim/cxx/cxx_object_file.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/cxx/cxx_object_file.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/cxx/cxx_object_file.cpp.o \
    -std=c++14 &
echo Compile src/doim/cxx/cxx_program.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/cxx/cxx_program.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/cxx/cxx_program.cpp.o \
    -std=c++14 &
echo Compile src/doim/cxx/cxx_static_library.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/cxx/cxx_static_library.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/cxx/cxx_static_library.cpp.o \
    -std=c++14 &
if [ ! -e build/release/src/doim/db/ ]; then mkdir build/release/src/doim/db/; fi
echo Compile src/doim/db/db_key.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/db/db_key.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/db/db_key.cpp.o -std=c++14 &
if [ ! -e build/release/src/doim/fs/ ]; then mkdir build/release/src/doim/fs/; fi
echo Compile src/doim/fs/fs_directory.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/fs/fs_directory.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/fs/fs_directory.cpp.o \
    -std=c++14 &
echo Compile src/doim/fs/fs_file.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/fs/fs_file.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/fs/fs_file.cpp.o -std=c++14 &
echo Compile src/doim/fs/fs_relative_directory.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/fs/fs_relative_directory.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ \
    -o build/release/src/doim/fs/fs_relative_directory.cpp.o -std=c++14 &
echo Compile src/doim/fs/fs_relative_file.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/fs/fs_relative_file.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/fs/fs_relative_file.cpp.o \
    -std=c++14 &
if [ ! -e build/release/src/doim/generic/ ]; then mkdir build/release/src/doim/generic/; fi
echo Compile src/doim/generic/attribute.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/generic/attribute.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/generic/attribute.cpp.o \
    -std=c++14 &
echo Compile src/doim/generic/attribute_name.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/generic/attribute_name.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/generic/attribute_name.cpp.o \
    -std=c++14 &
echo Compile src/doim/generic/attribute_value.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/generic/attribute_value.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/generic/attribute_value.cpp.o \
    -std=c++14 &
echo Compile src/doim/generic/object.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/generic/object.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/generic/object.cpp.o \
    -std=c++14 &
if [ ! -e build/release/src/doim/protobuf/ ]; then mkdir build/release/src/doim/protobuf/; fi
echo Compile src/doim/protobuf/protobuf_file.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/protobuf/protobuf_file.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/protobuf/protobuf_file.cpp.o \
    -std=c++14 &
if [ ! -e build/release/src/doim/sys/ ]; then mkdir build/release/src/doim/sys/; fi
echo Compile src/doim/sys/sys_argument.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/sys/sys_argument.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/sys/sys_argument.cpp.o \
    -std=c++14 &
echo Compile src/doim/sys/sys_command.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/sys/sys_command.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/sys/sys_command.cpp.o \
    -std=c++14 &
echo Compile src/doim/sys/sys_executable.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/sys/sys_executable.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/sys/sys_executable.cpp.o \
    -std=c++14 &
if [ ! -e build/release/src/doim/tag/ ]; then mkdir build/release/src/doim/tag/; fi
echo Compile src/doim/tag/tag.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/tag/tag.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/tag/tag.cpp.o -std=c++14 &
echo Compile src/doim/tag/tag_expression.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/tag/tag_expression.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/tag/tag_expression.cpp.o \
    -std=c++14 &
if [ ! -e build/release/src/doim/tool/ ]; then mkdir build/release/src/doim/tool/; fi
echo Compile src/doim/tool/tool_command.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/doim/tool/tool_command.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/doim/tool/tool_command.cpp.o \
    -std=c++14 &
if [ ! -e build/release/src/dom/ ]; then mkdir build/release/src/dom/; fi
if [ ! -e build/release/src/dom/cxx/ ]; then mkdir build/release/src/dom/cxx/; fi
echo Compile src/dom/cxx/cxx_library.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/dom/cxx/cxx_library.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/dom/cxx/cxx_library.cpp.o \
    -std=c++14 &
echo Compile src/dom/cxx/cxx_program.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/dom/cxx/cxx_program.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/dom/cxx/cxx_program.cpp.o \
    -std=c++14 &
if [ ! -e build/release/src/dom/protobuf/ ]; then mkdir build/release/src/dom/protobuf/; fi
echo Compile src/dom/protobuf/protobuf_plugin.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/dom/protobuf/protobuf_plugin.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/dom/protobuf/protobuf_plugin.cpp.o \
    -std=c++14 &
if [ ! -e build/release/src/engine/ ]; then mkdir build/release/src/engine/; fi
echo Compile src/engine/cxx_engine.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/engine/cxx_engine.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/engine/cxx_engine.cpp.o -std=c++14 &
if [ ! -e build/release/src/err/ ]; then mkdir build/release/src/err/; fi
echo Compile src/err/err.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/err/err.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/err/err.cpp.o -std=c++14 &
if [ ! -e build/release/src/log/ ]; then mkdir build/release/src/log/; fi
echo Compile src/log/hex.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/log/hex.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/log/hex.cpp.o -std=c++14 &
echo Compile src/log/log.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/log/log.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/log/log.cpp.o -std=c++14 &
if [ ! -e build/release/src/logex/ ]; then mkdir build/release/src/logex/; fi
echo Compile src/logex/log.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/logex/log.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/logex/log.cpp.o -std=c++14 &
echo Compile src/main.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/main.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem protobuf/include/ -isystem rocksdb/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/main.cpp.o -std=c++14 &
if [ ! -e build/release/src/option/ ]; then mkdir build/release/src/option/; fi
echo Compile src/option/verbose.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/option/verbose.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/option/verbose.cpp.o -std=c++14 &
if [ ! -e build/release/src/parser/ ]; then mkdir build/release/src/parser/; fi
if [ ! -e build/release/src/parser/cxx/ ]; then mkdir build/release/src/parser/cxx/; fi
echo Compile src/parser/cxx/cxx_parser.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/parser/cxx/cxx_parser.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/parser/cxx/cxx_parser.cpp.o -std=c++14 &
if [ ! -e build/release/src/parser/dbs/ ]; then mkdir build/release/src/parser/dbs/; fi
echo Compile src/parser/dbs/dbs_config_parser.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/parser/dbs/dbs_config_parser.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/parser/dbs/dbs_config_parser.cpp.o -std=c++14 &
echo Compile src/parser/dbs/dbs_parser.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/parser/dbs/dbs_parser.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/parser/dbs/dbs_parser.cpp.o -std=c++14 &
if [ ! -e build/release/src/rpc/client/ ]; then mkdir build/release/src/rpc/client/; fi
echo Compile src/rpc/client/client.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/rpc/client/client.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem protobuf/include/ -isystem src/system/ \
    -o build/release/src/rpc/client/client.cpp.o -std=c++14 &
echo Compile src/rpc/rpc.pb.cc
clang++ -D NDEBUG -I src/ -O3 -c src/rpc/rpc.pb.cc -isystem protobuf/include/ \
    -isystem src/system/ -o build/release/src/rpc/rpc.pb.cc.o -std=c++14 &
if [ ! -e build/release/src/rpc/server/ ]; then mkdir build/release/src/rpc/server/; fi
echo Compile src/rpc/server/server.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/rpc/server/server.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem protobuf/include/ -isystem src/system/ \
    -o build/release/src/rpc/server/server.cpp.o -std=c++14 &
if [ ! -e build/release/src/task/ ]; then mkdir build/release/src/task/; fi
if [ ! -e build/release/src/task/cxx/ ]; then mkdir build/release/src/task/cxx/; fi
echo Compile src/task/cxx/cxx_object_file_crc_task.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/task/cxx/cxx_object_file_crc_task.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/cxx/cxx_object_file_crc_task.cpp.o -std=c++14 &
echo Compile src/task/cxx/cxx_program_crc_task.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/task/cxx/cxx_program_crc_task.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/cxx/cxx_program_crc_task.cpp.o -std=c++14 &
echo Compile src/task/cxx/cxx_source_crc_task.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/task/cxx/cxx_source_crc_task.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/cxx/cxx_source_crc_task.cpp.o -std=c++14 &
echo Compile src/task/cxx/cxx_source_headers_task.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/task/cxx/cxx_source_headers_task.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/cxx/cxx_source_headers_task.cpp.o -std=c++14 &
if [ ! -e build/release/src/task/db/ ]; then mkdir build/release/src/task/db/; fi
echo Compile src/task/db/db_put_task.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/task/db/db_put_task.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/db/db_put_task.cpp.o -std=c++14 &
if [ ! -e build/release/src/task/protobuf/ ]; then mkdir build/release/src/task/protobuf/; fi
echo Compile src/task/protobuf/protobuf_file_crc_task.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/task/protobuf/protobuf_file_crc_task.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/protobuf/protobuf_file_crc_task.cpp.o -std=c++14 &
if [ ! -e build/release/src/task/sys/ ]; then mkdir build/release/src/task/sys/; fi
echo Compile src/task/sys/ensure_directory_task.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/task/sys/ensure_directory_task.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/sys/ensure_directory_task.cpp.o -std=c++14 &
echo Compile src/task/sys/execute_command_task.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/task/sys/execute_command_task.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/sys/execute_command_task.cpp.o -std=c++14 &
echo Compile src/task/sys/parse_stdout_task.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/task/sys/parse_stdout_task.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/sys/parse_stdout_task.cpp.o -std=c++14 &
echo Compile src/task/tpool.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/task/tpool.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/task/tpool.cpp.o -std=c++14 &
if [ ! -e build/release/src/tool/ ]; then mkdir build/release/src/tool/; fi
if [ ! -e build/release/src/tool/cxx/ ]; then mkdir build/release/src/tool/cxx/; fi
echo Compile src/tool/cxx/cxx_clang_format.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/tool/cxx/cxx_clang_format.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/tool/cxx/cxx_clang_format.cpp.o -std=c++14 &
echo Compile src/tool/cxx/cxx_compiler.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/tool/cxx/cxx_compiler.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/tool/cxx/cxx_compiler.cpp.o -std=c++14 &
echo Compile src/tool/cxx/cxx_iwyu.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/tool/cxx/cxx_iwyu.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/tool/cxx/cxx_iwyu.cpp.o -std=c++14 &
if [ ! -e build/release/src/tool/protobuf/ ]; then mkdir build/release/src/tool/protobuf/; fi
echo Compile src/tool/protobuf/protobuf_compiler.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/tool/protobuf/protobuf_compiler.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem axe/include/ -isystem boost/include/ -isystem fmt/include/ \
    -isystem rocksdb/include/ -isystem spdlog/include/ -isystem src/system/ \
    -o build/release/src/tool/protobuf/protobuf_compiler.cpp.o -std=c++14 &
if [ ! -e build/release/src/tpool/ ]; then mkdir build/release/src/tpool/; fi
echo Compile src/tpool/task.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/tpool/task.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/tpool/task.cpp.o -std=c++14 &
echo Compile src/tpool/task_callback.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/tpool/task_callback.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/tpool/task_callback.cpp.o \
    -std=c++14 &
echo Compile src/tpool/task_group.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/tpool/task_group.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/tpool/task_group.cpp.o -std=c++14 &
echo Compile src/tpool/tpool.cpp
clang++ -D NDEBUG -I src/ -O3 -c src/tpool/tpool.cpp \
    -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/ \
    -isystem boost/include/ -isystem fmt/include/ -isystem spdlog/include/ \
    -isystem src/system/ -o build/release/src/tpool/tpool.cpp.o -std=c++14 &
wait
echo Link build/release/dbs
clang++ -o build/release/dbs -stdlib=libc++ boost/lib/libboost_filesystem.a \
    boost/lib/libboost_system.a boost/lib/libboost_thread.a \
    build/release/src/const/constants.cpp.o build/release/src/db/database.cpp.o \
    build/release/src/doim/cxx/cxx_file.cpp.o \
    build/release/src/doim/cxx/cxx_header.cpp.o \
    build/release/src/doim/cxx/cxx_include_directory.cpp.o \
    build/release/src/doim/cxx/cxx_object_file.cpp.o \
    build/release/src/doim/cxx/cxx_program.cpp.o \
    build/release/src/doim/cxx/cxx_static_library.cpp.o \
    build/release/src/doim/db/db_key.cpp.o \
    build/release/src/doim/fs/fs_directory.cpp.o \
    build/release/src/doim/fs/fs_file.cpp.o \
    build/release/src/doim/fs/fs_relative_directory.cpp.o \
    build/release/src/doim/fs/fs_relative_file.cpp.o \
    build/release/src/doim/generic/attribute.cpp.o \
    build/release/src/doim/generic/attribute_name.cpp.o \
    build/release/src/doim/generic/attribute_value.cpp.o \
    build/release/src/doim/generic/object.cpp.o \
    build/release/src/doim/protobuf/protobuf_file.cpp.o \
    build/release/src/doim/sys/sys_argument.cpp.o \
    build/release/src/doim/sys/sys_command.cpp.o \
    build/release/src/doim/sys/sys_executable.cpp.o \
    build/release/src/doim/tag/tag.cpp.o \
    build/release/src/doim/tag/tag_expression.cpp.o \
    build/release/src/doim/tool/tool_command.cpp.o \
    build/release/src/dom/cxx/cxx_library.cpp.o \
    build/release/src/dom/cxx/cxx_program.cpp.o \
    build/release/src/dom/protobuf/protobuf_plugin.cpp.o \
    build/release/src/engine/cxx_engine.cpp.o build/release/src/err/err.cpp.o \
    build/release/src/log/hex.cpp.o build/release/src/log/log.cpp.o \
    build/release/src/logex/log.cpp.o build/release/src/main.cpp.o \
    build/release/src/option/verbose.cpp.o \
    build/release/src/parser/cxx/cxx_parser.cpp.o \
    build/release/src/parser/dbs/dbs_config_parser.cpp.o \
    build/release/src/parser/dbs/dbs_parser.cpp.o \
    build/release/src/rpc/client/client.cpp.o build/release/src/rpc/rpc.pb.cc.o \
    build/release/src/rpc/server/server.cpp.o \
    build/release/src/task/cxx/cxx_object_file_crc_task.cpp.o \
    build/release/src/task/cxx/cxx_program_crc_task.cpp.o \
    build/release/src/task/cxx/cxx_source_crc_task.cpp.o \
    build/release/src/task/cxx/cxx_source_headers_task.cpp.o \
    build/release/src/task/db/db_put_task.cpp.o \
    build/release/src/task/protobuf/protobuf_file_crc_task.cpp.o \
    build/release/src/task/sys/ensure_directory_task.cpp.o \
    build/release/src/task/sys/execute_command_task.cpp.o \
    build/release/src/task/sys/parse_stdout_task.cpp.o \
    build/release/src/task/tpool.cpp.o \
    build/release/src/tool/cxx/cxx_clang_format.cpp.o \
    build/release/src/tool/cxx/cxx_compiler.cpp.o \
    build/release/src/tool/cxx/cxx_iwyu.cpp.o \
    build/release/src/tool/protobuf/protobuf_compiler.cpp.o \
    build/release/src/tpool/task.cpp.o \
    build/release/src/tpool/task_callback.cpp.o \
    build/release/src/tpool/task_group.cpp.o build/release/src/tpool/tpool.cpp.o \
    bzip2/lib/libbz2.a fmt/lib/libfmt.a protobuf/lib/libprotobuf-lite.a \
    rocksdb/lib/librocksdb.a zlib/lib/libz.a
