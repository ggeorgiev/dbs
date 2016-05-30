#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)
cd $BASEDIR

find src -name *.cpp -o -name *.h -o -name *.hpp -o -name *.proto \
    | xargs clang/bin/clang-format -sort-includes -i
    
clang/bin/clang-format -sort-includes -i src/system/shared_ptr
clang/bin/clang-format -sort-includes -i src/system/str
clang/bin/clang-format -sort-includes -i src/system/string_view
clang/bin/clang-format -sort-includes -i src/system/unordered
clang/bin/clang-format -sort-includes -i src/system/variant
