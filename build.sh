MAKEOPTS=""

basedir=$(readlink -f `git rev-parse --show-cdup` .)
cd $basedir
mkdir build
cd build
cmake $basedir -DCMAKE_BUILD_TYPE=Debug
make $MAKEOPTS

