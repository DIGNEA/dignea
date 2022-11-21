//
// Created by amarrero on 18/6/21.
//

#ifndef DIGNEA_NOVELTYSEARCH_H
#define DIGNEA_NOVELTYSEARCH_H

#include <dignea/distances/Distance.h>
#include <dignea/utilities/KNN.h>
#include <dignea/utilities/Sorter.h>
#include <dignea/utilities/exceptions/EmptyPopulation.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

#include "Search.h"

using namespace std;
using json = nlohmann::json;

/**
 * @brief Class to represent the Novelty Search Algorithm
 *
 * @tparam P
 * @tparam S
 */
template <typename S, typename T>
class NoveltySearch : public Search<S> {
   public:
    NoveltySearch() = default;

    explicit NoveltySearch(unique_ptr<Distance<T>> dist, const int &iterations,
                           const float &threshold = 2000, const int &k = 15);

    virtual ~NoveltySearch() = default;

    Front<S> getResults() override;

    vector<S> run(vector<S> &population, const Problem<S> *problem) override;

    virtual void cmpFinals(vector<S> &population);

    virtual void insertIntoArchive(const S &solution);

    inline const vector<S> &getArchive() const { return this->noveltyArchive; }

    virtual json to_json();

   protected:
    unique_ptr<Distance<T>> distance;
    vector<S> noveltyArchive;
    vector<S> finalSs;
    int iterations;
    int performedIterations;
    float threshold;
    int k;
    int includedSinceCheck;
    // Ratios para aumentar o disminuir el threshold
    static const float loThresRatio;
    static const float upThresRatio;
    static const float ratioChanges;
};

/**
 * @brief Percentage of threshold which is lowered in the
 * evolution when no new individuals are added to the noveltyArchive in several
 * iterations
 * @tparam P
 * @tparam S
 */
template <typename S, typename T>
const float NoveltySearch<S, T>::loThresRatio = 0.05;
/**
 * @brief Percentage of threshold which is increased in the
 * evolution when multiple individuals are added to the noveltyArchive in
 * several iterations
 * @tparam P
 * @tparam S
 */
template <typename S, typename T>
const float NoveltySearch<S, T>::upThresRatio = 0.20;

/**
 * @brief Percentage of iterations to check noveltyArchive
 * @tparam P
 * @tparam S
 */
template <typename S, typename T>
const float NoveltySearch<S, T>::ratioChanges = 10;

/**
 * @brief Creates a new Novelty Search instance
 * @tparam P
 * @tparam S
 * @param threshold
 * @param dist
 * @param k
 */
template <typename S, typename T>
NoveltySearch<S, T>::NoveltySearch(unique_ptr<Distance<T>> dist,
                                   const int &iterations,
                                   const float &threshold, const int &k)
    : distance(move(dist)),
      iterations(iterations),
      performedIterations(0),
      threshold(threshold),
      k(k),
      includedSinceCheck(0) {}

/**
 * @brief Novelty Search Algorithm
 *      It looks for novelty using the genotypes of the individuals in the
 * population.
 *      Computes the sparseness of each individual in the noveltyArchive
 * and population
 * @tparam Problem
 * @tparam S
 * @tparam T
 * @param population
 * @param problem
 * @return
 */
template <typename S, typename T>
vector<S> NoveltySearch<S, T>::run(vector<S> &population,
                                   const Problem<S> *problem) {
    vector<S> combinedPop = population;
    combinedPop.insert(combinedPop.end(), noveltyArchive.begin(),
                       noveltyArchive.end());
    vector<T> spars = sparseness(combinedPop, distance.get(), k);
    for (unsigned int i = 0; i < population.size(); i++) {
        population[i].setDiversity(spars[i]);
    }
    return population;
}

/**
 * @brief Compares the individuals in the population against the neighbors
 * inside the archive of final Ss. If the score is good enough the individual
 * will be included inside the archive of final Ss
 * @tparam Problem
 * @tparam S
 * @tparam T
 * @param population
 */
template <typename S, typename T>
void NoveltySearch<S, T>::cmpFinals(vector<S> &population) {
    if (population.size() == 0) {
        std::string where = "NoveltySearch::cmpFinals";
        throw EmptyPopulation(where);
    }
    // Includes fittest feasible individual if the archive is empty
    if (finalSs.empty()) {
        sortByFitness(population);
        for (S &sol : population) {
            if (sol.getFitness() > 0) {
                finalSs.push_back(population[0]);
                break;
            }
        }
    } else {
        // Includes feasible individuals with spars > threshold
        for (S &solution : population) {
            T spars = sparseness(solution, finalSs, distance.get(), 1);
            if ((solution.getBiasedFitness() > 0) &&
                (spars > this->threshold)) {
                finalSs.push_back(solution);
            }
        }
    }
}

/**
 * @brief Returns the results obtained by the Novelty Search in a Front object
 * @tparam Problem
 * @tparam S
 * @tparam T
 * @return
 */
template <typename S, typename T>
Front<S> NoveltySearch<S, T>::getResults() {
    return Front<S>(this->finalSs);
}

/**
 * @brief Method to insert a new individual into the noveltyArchive of novelty
 * solutions
 * @tparam Problem
 * @tparam S
 * @tparam T
 * @param S
 */
template <typename S, typename T>
void NoveltySearch<S, T>::insertIntoArchive(const S &solution) {
    this->noveltyArchive.push_back(solution);
}

template <typename S, typename T>
json NoveltySearch<S, T>::to_json() {
    json data;
    data["name"] = "Novelty Search";
    data["k"] = this->k;
    data["threshold"] = this->threshold;
    return data;
}

#endif  // DIGNEA_NOVELTYSEARCH_H
