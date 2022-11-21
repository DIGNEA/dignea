#!/bin/bash
# Commands to install DIGNEA in Archer2 

#echo "Loading GCC 10.2.0"
#module load gcc/10.2.0
echo "Loading Libraries..."
echo "MPI"
module load cray-mpich/8.1.4
echo "Clang 11"
module load cce/11.0.4
echo "Cmake"
module load cmake/3.18.4

echo "Compiling the code..."
export CC=cc
export CXX=CC
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build . --target all -- -j 14
