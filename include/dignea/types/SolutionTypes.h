//
// Created by amarrero on 2/12/20.
//

#ifndef DIGNEA_SOLUTIONTYPES_H
#define DIGNEA_SOLUTIONTYPES_H

#include <vector>

#include "dignea/core/Solution.h"

typedef Solution<float, float> FloatSolution;
typedef Solution<int, double> IntDoubleSolution;
typedef Solution<int, float> IntFloatSolution;
typedef Solution<char, float> BoolFloatSolution;

#endif  // DIGNEA_SOLUTIONTYPES_H
