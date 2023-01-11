#!/bin/bash

echo "Cleaning the previous build"
rm -rf build bin

echo "Creating the directory structure"
mkdir -p build && cd build

echo "Building DIGNEA with examples and tests"
cmake -DCMAKE_BUILD_TYPE=Release -DINCLUDE_EXAMPLES=True -DINCLUDE_TESTS=True .. && cmake --build . --target all -- -j 14
