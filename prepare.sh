# Download all submodules
git submodule init || exit 1
git submodule update || exit 1

#Build cmake
if [ ! -e cmake ]
then
   pushd 3rdparty/cmake || exit 1

   ./bootstrap --prefix=../../cmake
   make
   make install
   git clean -fdx

   popd
fi


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
   ../../cmake/bin/cmake \
      -G "Unix Makefiles" \
      -DCMAKE_INSTALL_PREFIX:PATH=../../clang \
      -DCMAKE_BUILD_TYPE=Release \
      ../llvm

   echo Make ...
   make -j 8 || exit 1

   echo Install ...
   make install || exit 1

   rm -rf clang-build

   rm -rf llvm/tools/clang
   rm -rf llvm/projects/libcxx
   rm -rf llvm/projects/libcxxabi

   popd
fi

