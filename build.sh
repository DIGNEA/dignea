#!/bin/bash

mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DINCLUDE_EXAMPLES=True -DINCLUDE_TESTS=True .. && cmake --build . --target all -- -j 14