/**
 * @file MutationTypes.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-10-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_MUTATIONTYPES_H
#define DIGNEA_MUTATIONTYPES_H

#include <string>

using namespace std;

/**
 * @brief Type of Mutation Operators implemented in dignea
 *
 */
enum class MutType { UniformAll, UniformOne, SwapMutation };

#endif  // DIGNEA_MUTATIONTYPES_H
