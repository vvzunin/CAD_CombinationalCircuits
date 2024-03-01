#!/bin/bash
currentDir=$(pwd)
cd $currentDir/Generator/source
mkdir build
cd build
cmake ..
make -j 10
cd $currentDir/Generator/source/build/tests
./tests
