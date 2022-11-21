//
// Created by amarrero on 5/4/21.
//

#ifndef DIGNEA_EVALUATORTYPES_H
#define DIGNEA_EVALUATORTYPES_H

#include <string>

using namespace std;

namespace Evaluator_Types {
using EvaluatorT = int;
const EvaluatorT SEQUENTIAL = 0;
const EvaluatorT PARALLEL_OMP = 1;
}  // namespace Evaluator_Types

#endif  // DIGNEA_EVALUATORTYPES_H
