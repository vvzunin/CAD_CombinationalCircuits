#!/bin/bash
cd Generator/source
mkdir build
cd build
cmake ..
make
cd ../../../
currentDir=$(pwd)
Generator/source/build/prog --json_path=$currentDir/Generator/source/test_data/wast/sample.json
