#!/bin/sh

cd ext-lib/cyassl
./autogen.sh
./configure --host= --disable-shared --prefix=$PWD/../..
make clean
make
make install

cd ../..
cd build
make clean
make all

exit 0
