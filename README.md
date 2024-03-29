<center>
  <h1>DIGNEA</h1>
  <h4>Diverse Instance Generator with Novelty Search and Evolutionary Algorithms</h4>
  
[![Build](https://github.com/DIGNEA/dignea/actions/workflows/cmake.yml/badge.svg)](https://github.com/DIGNEA/dignea/actions/workflows/cmake.yml/badge.svg)
[![Test](https://github.com/DIGNEA/dignea/actions/workflows/catch.yml/badge.svg)](https://github.com/DIGNEA/dignea/actions/workflows/catch.yml//badge.svg)
[![Coverage Status](https://coveralls.io/repos/github/DIGNEA/dignea/badge.svg?branch=main)](https://coveralls.io/github/DIGNEA/dignea?branch=main)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Documentation](https://github.com/DIGNEA/dignea/actions/workflows/pages/pages-build-deployment/badge.svg)](https://github.com/DIGNEA/dignea/actions/workflows/pages/pages-build-deployment//badge.svg)
</center>



Repository containing DIGNEA, a Diverse Instance Generator with Novelty Search and Evolutionary Algorithms. This framework is an extensible tool for generating diverse and discriminatory instances for any desired domain. The instances obtained generated will be biased to the performance of a *target* in a specified portfolio of algorithms. 

## Build DIGNEA 

For building DIGNEA you can just simply clone and build the project. Make sure that you have all the dependencies installed in your machine.

```bash 
  git clone git@github.com:DIGNEA/dignea.git
  cd dignea
  ./build.sh
```

If you want to install DIGNEA in you machine run the following command:

```bash
sudo cmake --build path-to-dignea-build-dir/ --config Release --target install --
```

### Docker Image

For your convinience, you can find a Docker image of the building enviroment plus the source code of DIGNEA published at Docker Hub [here](https://hub.docker.com/r/dignea/dignea). Follow the steps described in the tutorial provided to start using the software.


### Dependencies

- GCC >= 10.0.0
- CMake >= 3.14
- C++20 support
- OpenMP support
    

## Publications

DIGNEA was used in the following publications:

* Alejandro Marrero, Eduardo Segredo, and Coromoto Leon. 2021. A parallel genetic algorithm to speed up the resolution of the algorithm selection problem. Proceedings of the Genetic and Evolutionary Computation Conference Companion. Association for Computing Machinery, New York, NY, USA, 1978–1981. DOI:https://doi.org/10.1145/3449726.3463160

* Marrero, A., Segredo, E., León, C., Hart, E. (2022). A Novelty-Search Approach to Filling an Instance-Space with Diverse and Discriminatory Instances for the Knapsack Problem. In: Rudolph, G., Kononova, A.V., Aguirre, H., Kerschke, P., Ochoa, G., Tušar, T. (eds) Parallel Problem Solving from Nature – PPSN XVII. PPSN 2022. Lecture Notes in Computer Science, vol 13398. Springer, Cham. https://doi.org/10.1007/978-3-031-14714-2_16


  
