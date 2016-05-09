#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)

cd $BASEDIR

if [ ! -e clang -o ! "$(ls -A clang)" ]
then
    curl -L https://github.com/ggeorgiev/dbs/releases/download/clang/clang-osx.zip > clang.zip
    unzip clang.zip
    rm  clang.zip
fi

if [ ! -e gtest -o ! "$(ls -A gtest)" ]
then
    git submodule update --init 3rdparty/gtest || exit 1

    cd 3rdparty/gtest || exit 1

    echo Build gtest ...

    rm -rf build || exit 1
    mkdir build || exit 1
    cd build || exit 1

    cmake \
        -G "Unix Makefiles" \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DBUILD_SHARED_LIBS=OFF \
        -Wno-dev \
        -Dgtest_build_samples=ON \
        .. || exit 1

    make || exit 1

    cd ..

    mkdir -p ../../gtest/include || exit 1
    cp -r include/gtest ../../gtest/include | exit 1

    mkdir -p ../../gtest/lib || exit 1
    cp build/lib*.a ../../gtest/lib || exit 1

    git clean -fdx

    cd ../..
fi

if [ ! -e boost -o ! "$(ls -A boost)" ]
then
    git submodule update --init 3rdparty/boost || exit 1

    cd 3rdparty/boost || exit 1

    git submodule update --init tools/build          || exit 1
    git submodule update --init tools/inspect        || exit 1
    git submodule update --init libs/algorithm       || exit 1
    git submodule update --init libs/array           || exit 1
    git submodule update --init libs/assert          || exit 1
    git submodule update --init libs/atomic          || exit 1
    git submodule update --init libs/bind            || exit 1
    git submodule update --init libs/chrono          || exit 1
    git submodule update --init libs/concept_check   || exit 1
    git submodule update --init libs/config          || exit 1
    git submodule update --init libs/container       || exit 1
    git submodule update --init libs/core            || exit 1
    git submodule update --init libs/crc             || exit 1
    git submodule update --init libs/date_time       || exit 1
    git submodule update --init libs/detail          || exit 1
    git submodule update --init libs/exception       || exit 1
    git submodule update --init libs/filesystem      || exit 1
    git submodule update --init libs/function        || exit 1
    git submodule update --init libs/functional      || exit 1
    git submodule update --init libs/fusion          || exit 1
    git submodule update --init libs/heap            || exit 1
    git submodule update --init libs/integer         || exit 1
    git submodule update --init libs/intrusive       || exit 1
    git submodule update --init libs/io              || exit 1
    git submodule update --init libs/iterator        || exit 1
    git submodule update --init libs/lexical_cast    || exit 1
    git submodule update --init libs/math            || exit 1
    git submodule update --init libs/mpl             || exit 1
    git submodule update --init libs/move            || exit 1
    git submodule update --init libs/numeric         || exit 1
    git submodule update --init libs/optional        || exit 1
    git submodule update --init libs/parameter       || exit 1
    git submodule update --init libs/predef          || exit 1
    git submodule update --init libs/preprocessor    || exit 1
    git submodule update --init libs/range           || exit 1
    git submodule update --init libs/ratio           || exit 1
    git submodule update --init libs/throw_exception || exit 1
    git submodule update --init libs/thread          || exit 1
    git submodule update --init libs/tuple           || exit 1
    git submodule update --init libs/type_traits     || exit 1
    git submodule update --init libs/smart_ptr       || exit 1
    git submodule update --init libs/static_assert   || exit 1
    git submodule update --init libs/system          || exit 1
    git submodule update --init libs/utility         || exit 1

    git submodule foreach -q 'git checkout boost-1.59.0'

    echo Build boost ...

    ./bootstrap.sh || exit 1

    SYSROOT="/Applications/Xcode.app/Contents/Developer/Platforms"
    SYSROOT="$SYSROOT/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk"

    build_boost() {
        BOOSTBUILD=\
            ./b2 \
            toolset=clang \
            cxxflags="-std=c++11 -stdlib=libc++ -isystem=$SYSROOT" \
            linkflags="-stdlib=libc++" \
            --layout=system \
            --prefix=../../boost \
            --with-chrono --with-filesystem --with-system --with-thread \
            threading=multi link=static $1
    }

    build_boost headers || exit 1
    build_boost install || exit 1

    git clean -fdx

    cd libs/config
    git clean -fdx

    cd ../..

    cd ../..
fi

if [ ! -e fmt -o ! "$(ls -A fmt)" ]
then
    git submodule update --init 3rdparty/fmt || exit 1

    cd 3rdparty/fmt || exit 1

    echo Build fmt ...

    mkdir build
    cd build

    cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_INSTALL_PREFIX:PATH=$BASEDIR/fmt .. || exit 1
    CXX=clang++ make || exit 1
    make test || exit 1
    make install || exit 1

    cd ..

    git clean -fdx

    cd ../.. || exit 1
fi

if [ ! -e spdlog -o ! "$(ls -A spdlog)" ]
then
    git submodule update --init 3rdparty/spdlog || exit 1

    echo Build spdlog ...

    mkdir -p spdlog || exit 1

    cp -r 3rdparty/spdlog/include spdlog || exit 1
fi

if [ ! -e rocksdb -o ! "$(ls -A rocksdb)" ]
then
    git submodule update --init 3rdparty/rocksdb || exit 1

    cd 3rdparty/rocksdb || exit 1

    echo Build RocksDB ...

    CXX=clang++ make static_lib || exit 1

    mkdir -p ../../rocksdb/lib
    cp -r include ../../rocksdb || exit 1
    cp librocksdb.a ../../rocksdb/lib || exit 1

    git clean -fdx

    cd ../.. || exit 1
fi


echo Update git hooks
cp git-hooks/* .git/hooks/
