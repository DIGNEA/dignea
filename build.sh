#!/bin/bash

cmake -B  -DCMAKE_BUILD_TYPE=Release -DINCLUDE_EXAMPLES=True -DINCLUDE_TESTS=True
cmake --build . --target all --
