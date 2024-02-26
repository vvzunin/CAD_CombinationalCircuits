#!/bin/bash
cd Generator/source
mkdir build
cd build
cmake ..
make
cd ../../../
currentDir=$(pwd)
gdb --args Generator/source/build/circuitgen --json_path=$currentDir/Generator/source/test_data/wast/sample.json
