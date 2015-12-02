#!/bin/sh

BASEDIR=$(pwd)/$(dirname $0)

cd $BASEDIR

# Download all submodules
git submodule update --init || exit 1

#Build cmake
if [ ! -e cmake ]
then
    pushd 3rdparty/cmake || exit 1

    ./bootstrap --prefix=$BASEDIR/cmake
    make
    make install
    git clean -fdx

    popd
fi

CMAKE=$BASEDIR/cmake/bin/cmake

if [ ! -e clang ]
then
    pushd 3rdparty || exit 1

    echo Build clang ...

    echo Copy clang to llvm ...
    rm -rf llvm/tools/clang
    mkdir -p llvm/tools/clang || exit 1
    cp -r clang llvm/tools || exit 1

    echo Copy clang extra tools to llvm ...
    rm -rf llvm/tools/clang/tools/extra
    mkdir -p llvm/tools/clang/tools/extra || exit 1
    cp -r clang-tools-extra/ llvm/tools/clang/tools/extra/ || exit 1

    echo Copy libcxx to llvm ...
    rm -rf llvm/projects/libcxx
    mkdir -p llvm/projects/libcxx || exit 1
    cp -r libcxx llvm/projects || exit 1

    echo Copy libcxxabi to llvm ...
    rm -rf llvm/projects/libcxxabi
    mkdir -p llvm/projects/libcxxabi || exit 1
    cp -r libcxxabi llvm/projects || exit 1

    rm -rf clang-build || exit 1
    mkdir clang-build || exit 1
    cd clang-build || exit 1

    echo Prepare make files ...
    $CMAKE \
        -G "Unix Makefiles" \
        -DCMAKE_INSTALL_PREFIX:PATH=../../clang \
        -DCMAKE_BUILD_TYPE=Release \
        ../llvm || exit 1

    echo Make ...
    make || exit 1

    echo Install ...
    make install || exit 1

    cd ..
    rm -rf clang-build

    rm -rf llvm/tools/clang
    rm -rf llvm/projects/libcxx
    rm -rf llvm/projects/libcxxabi

    popd
fi

CLANGDIR=$BASEDIR/clang
CLANGBIN=$CLANGDIR/bin
CLANG=$CLANGBIN/clang++

if [ ! -e iwyu ]
then
    pushd 3rdparty/iwyu || exit 1

    echo Build Include what you use ...

    rm -rf build || exit 1
    mkdir build || exit 1
    cd build || exit 1

    $CMAKE \
        -G "Unix Makefiles" \
        -DLLVM_PATH=$CLANGDIR \
        -DCMAKE_INSTALL_PREFIX:PATH=../../../iwyu \
        .. || exit 1

    make || exit 1
    make install || exit 1

    cd ..

    git clean -fdx

    popd

    cp iwyu/bin/* $CLANGBIN
fi

if [ ! -e gtest ]
then
    pushd 3rdparty/gtest || exit 1

    echo Build gtest ...

    rm -rf build || exit 1
    mkdir build || exit 1
    cd build || exit 1

    $CMAKE \
        -G "Unix Makefiles" \
        -DCMAKE_CXX_COMPILER=$CLANG \
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

    popd
fi

if [ ! -e boost ]
then
    pushd 3rdparty/boost || exit 1

    git submodule update --init tools/build          || exit 1
    git submodule update --init tools/inspect        || exit 1
    git submodule update --init libs/array           || exit 1
    git submodule update --init libs/assert          || exit 1
    git submodule update --init libs/chrono          || exit 1
    git submodule update --init libs/concept_check   || exit 1
    git submodule update --init libs/config          || exit 1
    git submodule update --init libs/container       || exit 1
    git submodule update --init libs/core            || exit 1
    git submodule update --init libs/detail          || exit 1
    git submodule update --init libs/functional      || exit 1
    git submodule update --init libs/integer         || exit 1
    git submodule update --init libs/iterator        || exit 1
    git submodule update --init libs/lexical_cast    || exit 1
    git submodule update --init libs/math            || exit 1
    git submodule update --init libs/mpl             || exit 1
    git submodule update --init libs/numeric         || exit 1
    git submodule update --init libs/predef          || exit 1
    git submodule update --init libs/preprocessor    || exit 1
    git submodule update --init libs/range           || exit 1
    git submodule update --init libs/ratio           || exit 1
    git submodule update --init libs/throw_exception || exit 1
    git submodule update --init libs/type_traits     || exit 1
    git submodule update --init libs/static_assert   || exit 1
    git submodule update --init libs/system          || exit 1
    git submodule update --init libs/utility         || exit 1

    git submodule foreach -q 'git checkout boost-1.59.0'

    echo Build boost ...

    ./bootstrap.sh || exit 1

    build_boost() {
        BOOSTBUILD=PATH=$CLANGBIN:$PATH \
            ./b2 \
            toolset=clang \
            cxxflags="-std=c++11 -stdlib=libc++ -isystem/Developer/SDKs/MacOSX10.7.sdk/usr/include" \
            linkflags="-stdlib=libc++" \
            --layout=system \
            --prefix=../../boost \
            --with-system --with-chrono \
            threading=multi link=static $1
    }

    build_boost headers || exit 1
    build_boost install || exit 1

    git clean -fdx

    cd libs/config
    git clean -fdx

    popd
fi

if [ ! -e cppformat ]
then
    pushd 3rdparty/cppformat || exit 1

    echo Build cppformat ...

    mkdir build
    cd build

    $CMAKE -DCMAKE_INSTALL_PREFIX:PATH=../../../cppformat .. || exit 1
    make || exit 1
    make test || exit 1
    make install || exit 1

    cd ..

    git clean -fdx

    popd

    mv cppformat/include/ cppformat/cppformat || exit 1
    mkdir -p cppformat/include || exit 1
    mv cppformat/cppformat cppformat/include || exit 1
fi


echo Update git hooks
cp git-hooks/* .git/hooks/
