//
// Created by amarrero on 2/12/20.
//

#ifndef DIGNEA_SOLUTIONTYPES_H
#define DIGNEA_SOLUTIONTYPES_H

#include <vector>

#include "dignea/core/Solution.h"

/**
 * @brief Solution which is represented as a vector of floats (chromosome) and
 * the objectives values are also floats
 *
 */
typedef Solution<float, float> FloatSolution;
/**
 * @brief Solution which is represented as a vector of ints (chromosome) and the
 * objective values are doubles
 *
 */
typedef Solution<int, double> IntDoubleSolution;
/**
 * @brief Solution which is represented as a vector of ints (chromosome) and the
 * objective values are floats
 *
 */
typedef Solution<int, float> IntFloatSolution;
/**
 * @brief Solution which is represented as a vector of chars (booleans)
 * (chromosome) and the objectives are floats
 *
 */
typedef Solution<char, float> BoolFloatSolution;
/**
 * @brief Solution which is represented as a vector of ints (chromosome) and the
 * objective values are ints
 *
 */
typedef Solution<int, int> IntIntSolution;

#endif  // DIGNEA_SOLUTIONTYPES_H
