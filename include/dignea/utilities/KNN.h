/**
 * @file KNN.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-10-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __KNN_H__
#define __KNN_H__

#include <dignea/core/Solution.h>
#include <dignea/distances/Distance.h>
#include <dignea/searches/NoveltySearch.h>

#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

/**
 * @brief Data type which represents a close neighbours.
 * The first value is the distance to the individual
 * The second value is the index of the neighbour in the array
 * (Distance_to_other, other_index)
 * @tparam T
 */
using neighborT = pair<float, int>;

/**
 * @brief Vector of neighbours
 * Each element is of type neighborT.
 * [(Distance_to_other_0, other_index_0),
 *  (Distance_to_other_1, other_index_1),
 *  (...),
 *  (Distance_to_other_n-1, other_index_n-1)]
 * @tparam T
 */
using neighVector = vector<neighborT>;
/**
 * @brief Data type which represents a matrix of close neighbours.
 * The first value is the index of the neighbour in the array
 * The second value is the distance to the individual
 * [
 *  [
 *  (Distance_to_other_0, other_index_0),
 *  (Distance_to_other_1, other_index_1),
 *  (...),
 *  (Distance_to_other_n-1, other_index_n-1)
 *  ],
 *  [
 *  (Distance_to_other_0, other_index_0),
 *  (Distance_to_other_1, other_index_1),
 *  (...),
 *  (Distance_to_other_n-1, other_index_n-1)
 *  ]
 * ]
 * @tparam T
 */
using neighMatrix = vector<neighVector>;

using Descriptor = vector<float>;
/**
 * @brief K-Nearest Neighbours Algorithm
 */
class KNN {
   public:
    KNN() = default;

    virtual ~KNN() = default;

    neighVector run(const Descriptor &individual,
                    const vector<Descriptor> &neighbours,
                    Distance<float> *metric, const int &k = 3);

    neighMatrix run(const vector<Descriptor> &population,
                    Distance<float> *metric, const int &k = 3);

    neighMatrix inverse(const vector<Descriptor> &population,
                        Distance<float> *metric, const int &k = 3);
};

/**
 * @brief Computes the sparseness of all individuals in the population vector
 * Based on the Novelty Search from Exploiting Open-Endedness to Solve Problems
 * Through the Search for Novelty
 *
 * @tparam T
 * @param individual
 * @param neighbours
 * @param metric
 * @param k
 * @return T
 */
vector<float> sparseness(const vector<Descriptor> &population,
                         Distance<float> *metric, const int k);
/**
 * @brief Computes the sparseness score of the individual against the rest of
 * individuals in the population list
 * Based on the Novelty Search from Exploiting Open-Endedness to Solve Problems
 * Through the Search for Novelty
 *
 * @tparam T
 * @param individual
 * @param neighbours
 * @param metric
 * @param k
 * @return T
 */
float sparseness(const Descriptor &solution,
                 const vector<Descriptor> &population, Distance<float> *metric,
                 const int k);

#endif  // __KNN_H__