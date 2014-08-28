#!/bin/sh

sudo apt-get install git-core git-svn

echo "Cloning CLD2"
git svn clone http://cld2.googlecode.com/svn/trunk/ libcld2

echo "Compiling libcld2"
cd libcld2/internal
./compile_libs.sh

echo "Installing libcld2"
sudo cp libcld2.so /usr/local/lib
