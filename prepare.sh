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
    rm -rf llvm/tools/clang || exit 1
    mkdir llvm/tools/clang || exit 1
    cp -r clang llvm/tools || exit 1

    echo Copy libcxx to llvm ...
    rm -rf llvm/projects/libcxx || exit 1
    mkdir llvm/projects/libcxx || exit 1
    cp -r libcxx llvm/projects || exit 1

    echo Copy libcxxabi to llvm ...
    rm -rf llvm/projects/libcxxabi || exit 1
    mkdir llvm/projects/libcxxabi || exit 1
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
    make -j 8 || exit 1

    echo Install ...
    make install || exit 1

    cd ..
    rm -rf clang-build

    rm -rf llvm/tools/clang
    rm -rf llvm/projects/libcxx
    rm -rf llvm/projects/libcxxabi

    popd
fi

CLANGBIN=$BASEDIR/clang/bin
CLANG=$CLANGBIN/clang++

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

    popd

    mkdir -p gtest/include || exit 1
    cp -r 3rdparty/gtest/include/gtest gtest/include | exit 1

    mkdir -p gtest/lib || exit 1
    cp 3rdparty/gtest/build/lib*.a gtest/lib || exit 1

    rm -rf 3rdparty/gtest/build || exit 1
fi

if [ ! -e boost ]
then
    pushd 3rdparty/boost || exit 1

    git submodule update --init tools/build     || exit 1
    git submodule update --init tools/inspect   || exit 1
    git submodule update --init libs/assert     || exit 1
    git submodule update --init libs/config     || exit 1
    git submodule update --init libs/core       || exit 1
    git submodule update --init libs/predef     || exit 1
    git submodule update --init libs/system     || exit 1
    git submodule update --init libs/utility    || exit 1

    echo Build boost ...

    ./bootstrap.sh --prefix=../../boost --includedir=../../boost/include || exit 1

    PATH=$CLANGBIN:$PATH \
        ./b2 \
        toolset=clang \
        cxxflags="-std=c++11 -stdlib=libc++ -isystem/Developer/SDKs/MacOSX10.7.sdk/usr/include" \
        linkflags="-stdlib=libc++" \
        install threading=multi link=static

    git clean -fdx

    cd libs/config
    git clean -fdx

    popd
fi


echo Update git hooks
cp git-hooks/* .git/hooks/
