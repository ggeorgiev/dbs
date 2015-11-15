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

   popd
fi


#Build clang
pushd 3rdparty/llvm || exit 1

if [ ! -e tools/clang ]
then
   ln -s ../clang tools/clang || exit 1
fi

if [ ! -e build ]
then
   mkdir build || exit 1
fi

cd build || exit 1

popd

