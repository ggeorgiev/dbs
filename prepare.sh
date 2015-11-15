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


#Build clang
if [ ! -e clang ]
then
   pushd 3rdparty || exit 1

   if [ ! -e llvm/tools/clang ]
   then
      cp -r clang llvm/tools/clang || exit 1
   fi

   if [ ! -e clang-build ]
   then
      mkdir clang-build || exit 1
   fi

   cd clang-build || exit 1

   ../../cmake/bin/cmake \
      -G "Unix Makefiles" \
      -DCMAKE_INSTALL_PREFIX:PATH=../../clang \
      -DCMAKE_BUILD_TYPE=Release \
      ../llvm

   make || exit 1
   make install || exit 1

   rm -rf llvm/tools/clang

   popd
fi

