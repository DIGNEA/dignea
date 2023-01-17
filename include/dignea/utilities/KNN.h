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
template <typename T>
using neighborT = pair<T, int>;

/**
 * @brief Vector of neighbours
 * Each element is of type neighborT.
 * [(Distance_to_other_0, other_index_0),
 *  (Distance_to_other_1, other_index_1),
 *  (...),
 *  (Distance_to_other_n-1, other_index_n-1)]
 * @tparam T
 */
template <typename T>
using neighVector = vector<neighborT<T>>;
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
template <typename T>
using neighMatrix = vector<neighVector<T>>;

/**
 * @brief K-Nearest Neighbours Algorithm
 */
class KNN {
   public:
    KNN() = default;

    virtual ~KNN() = default;

    template <typename S, typename T>
    neighVector<T> run(const S &individual, const vector<S> &neighbours,
                       Distance<T> *metric, const int &k = 3);

    template <typename S, typename T>
    neighMatrix<T> run(const vector<S> &population, Distance<T> *metric,
                       const int &k = 3);

    template <typename T>
    neighMatrix<T> run(const vector<vector<float>> &population,
                       Distance<T> *metric, const int &k = 3);

    template <typename S, typename T>
    neighMatrix<T> inverse(const vector<S> &population, Distance<T> *metric,
                           const int &k = 3);
};

/**
 * @brief Computes the KNN algorithm for the individual
 * Returns the k nearest neighbours Ss in a vector
 *
 * @tparam T
 * @tparam S
 * @param individual
 * @param neighbours
 * @param metric
 * @param k
 * @return vector<S>
 */
template <typename S, typename T>
neighVector<T> KNN::run(const S &individual, const vector<S> &neighbours,
                        Distance<T> *metric, const int &k) {
    if ((unsigned int)k > neighbours.size()) {
        throw runtime_error(
            "K is " + to_string(k) +
            " and neighbours are only: " + to_string(neighbours.size()));
    }
    neighVector<T> distances = {};
    distances.reserve(neighbours.size());
    vector indVarsAuto = individual.getVariables();
    vector<T> indVars(indVarsAuto.begin(), indVarsAuto.end());
    for (unsigned int i = 0; i < neighbours.size(); i++) {
        vector NvarsAuto = neighbours[i].getVariables();
        std::vector<T> neighVars(NvarsAuto.begin(), NvarsAuto.end());
        distances.push_back({metric->compute(indVars, neighVars), i});
    }
    sort(distances.begin(), distances.end());
    return {distances.begin(), distances.begin() + k};
}

/**
 * @brief Computes the KNN for the entire vector of individuals given
 *
 * @tparam S
 * @tparam T
 * @param population
 * @param metric
 * @param k
 * @return neighMatrix<T>
 */
template <typename S, typename T>
neighMatrix<T> KNN::run(const vector<S> &population, Distance<T> *metric,
                        const int &k) {
    if ((unsigned int)k > population.size()) {
        throw runtime_error(
            "K is " + to_string(k) +
            " and population are only: " + to_string(population.size()));
    }
    neighMatrix<T> nearest;
    nearest.reserve(population.size());
    for (unsigned int i = 0; i < population.size(); i++) {
        neighVector<T> distances = {};
        distances.reserve(population.size() - 1);
        vector tempVars = population[i].getVariables();
        vector<T> iVars(tempVars.begin(), tempVars.end());
        for (unsigned int j = 0; j < population.size(); j++) {
            // For each other individual we compute the distance to i
            if (i == j) continue;
            vector tmpJVars = population[j].getVariables();
            vector<T> jVars(tmpJVars.begin(), tmpJVars.end());
            T dist = metric->compute(iVars, jVars);
            distances.push_back({dist, j});
        }
        // Ordenamos los vecinos de I por su distancia a I
        sort(distances.begin(), distances.end());
        // We only get the K nearest neighbors
        nearest.push_back({distances.begin(), distances.begin() + k});
    }
    return nearest;
}

/**
 * @brief Runs the KNN algorithm with a matrix of float values. This method is
 * defined to be used by EIG and NSFeatures and the matrix defines the features
 * of all individuals in the population (instances).
 *
 * @tparam T
 * @param population
 * @param metric
 * @param k
 * @return neighMatrix<T>
 */
template <typename T>
neighMatrix<T> KNN::run(const vector<vector<float>> &population,
                        Distance<T> *metric, const int &k) {
    if ((unsigned int)k > population.size()) {
        throw runtime_error(
            "K is " + to_string(k) +
            " and population are only: " + to_string(population.size()));
    }
    neighMatrix<T> nearest;
    nearest.reserve(population.size());
    for (unsigned int i = 0; i < population.size(); i++) {
        neighVector<T> distances = {};
        distances.reserve(population.size() - 1);
        for (unsigned int j = 0; j < population.size(); j++) {
            if (i == j) continue;
            T dist = metric->compute(population[i], population[j]);
            distances.push_back({dist, j});
        }
        // Ordenamos los vecinos de I por su distancia a I
        sort(distances.begin(), distances.end());
        nearest.push_back({distances.begin(), distances.begin() + k});
    }
    return nearest;
}

/**
 * @brief Computes the inverse of KNN
 *
 * @tparam S
 * @tparam T
 * @param population
 * @param metric
 * @param k
 * @return neighMatrix<T>
 */
template <typename S, typename T>
neighMatrix<T> KNN::inverse(const vector<S> &population, Distance<T> *metric,
                            const int &k) {
    if ((unsigned int)k > population.size()) {
        throw runtime_error(
            "K is " + to_string(k) +
            " and population are only: " + to_string(population.size()));
    }
    neighMatrix<T> nearest;
    nearest.reserve(population.size());
    for (unsigned int i = 0; i < population.size(); i++) {
        neighVector<T> distances = {};
        distances.reserve(population.size() - 1);
        for (unsigned int j = 0; j < population.size(); j++) {
            if (i == j) continue;
            T dist = metric->compute(population[i].getVariables(),
                                     population[j].getVariables());
            distances.push_back({dist, j});
        }
        // Ordenamos los vecinos de I por su distancia a I
        sort(distances.rbegin(), distances.rend());
        nearest.push_back({distances.begin(), distances.begin() + k});
    }
    return nearest;
}

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
template <typename T, typename S>
vector<T> sparseness(const vector<S> &population, Distance<T> *metric,
                     const int k = 3) {
    if ((unsigned int)k > population.size()) {
        throw runtime_error(
            "K is " + to_string(k) +
            " and population are only: " + to_string(population.size()));
    }
    auto knn = make_unique<KNN>();
    neighMatrix<T> nearest = knn->run(population, metric, k);
    vector<T> spars(population.size(), 0);
    float frac = (1 / static_cast<float>(k));
    for (unsigned int i = 0; i < population.size(); i++) {
        for (int j = 0; j < k; j++) {
            spars[i] += nearest[i][j].first;
        }
        spars[i] *= frac;
    }

    return spars;
}

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
template <typename T>
vector<float> sparseness(const vector<vector<float>> &population,
                         Distance<T> *metric, const int k = 3) {
    if ((unsigned int)k > population.size()) {
        throw runtime_error(
            "K is " + to_string(k) +
            " and population are only: " + to_string(population.size()));
    }
    auto knn = make_unique<KNN>();
    neighMatrix<T> nearest = knn->run(population, metric, k);
    vector<float> spars(population.size(), 0);
    float frac = (1 / static_cast<float>(k));
    for (unsigned int i = 0; i < population.size(); i++) {
        for (int j = 0; j < k; j++) {
            spars[i] += nearest[i][j].first;
        }
        spars[i] *= frac;
    }

    return spars;
}

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
template <typename T, typename S>
float sparseness(S &solution, const vector<S> &population, Distance<T> *metric,
                 const int k = 1) {
    if ((unsigned int)k > population.size()) {
        throw runtime_error(
            "K is " + to_string(k) +
            " and population are only: " + to_string(population.size()));
    }
    auto knn = make_unique<KNN>();
    neighVector<T> nearest = knn->run(solution, population, metric, k);
    float spars = 0.0;
    float frac = (1 / static_cast<float>(k));
    for (const neighborT<T> &neighborT : nearest) {
        spars += neighborT.first;
    }
    spars *= frac;
    return spars;
}

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
template <typename T>
float sparseness(vector<float> &solution,
                 const vector<vector<float>> &population, Distance<T> *metric,
                 const int k = 1) {
    auto knn = make_unique<KNN>();
    neighVector<T> nearest = knn->run(solution, population, metric, k);
    float spars = 0.0;
    float frac = (1 / static_cast<float>(k));
    for (const neighborT<T> &neighborT : nearest) {
        spars += neighborT.first;
    }
    spars *= frac;
    return spars;
}

#endif  // __KNN_H__
