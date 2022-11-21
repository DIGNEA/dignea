//
// Created by amarrero on 28/6/21.
//

#ifndef DIGNEA_NSFEATURES_H
#define DIGNEA_NSFEATURES_H

#include <dignea/distances/Distance.h>
#include <dignea/mea/MEASolution.h>
#include <dignea/searches/NoveltySearch.h>
#include <dignea/utilities/KNN.h>
#include <dignea/utilities/Sorter.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

#include "dignea/searches/Search.h"

using namespace std;
using json = nlohmann::json;

using vars = vector<float>;

/**
 * @brief Class to represent the Novelty Search Algorithm
 * This specialization is exclusively for MEA because it uses
 * the vector of features of each generated instance to compute novelty
 *
 * @tparam MS --> MEASolution type
 */
template <typename MS>
class NSFeatures : public NoveltySearch<MS, float> {
   public:
    NSFeatures() = default;

    explicit NSFeatures(unique_ptr<Distance<float>> dist, const int &iterations,
                        const float &threshold = 2000, const int &k = 15);

    virtual ~NSFeatures() = default;

    vector<MS> run(vector<MS> &population, const Problem<MS> *problem) override;

    void insertIntoArchive(const MS &solution);

    [[nodiscard]] vector<vars> getArchive() const { return noveltyFeatures; };

    virtual json to_json() override;

   protected:
    virtual vector<vars> beforeRun(vector<MS> &population);

   protected:
    vector<vars> noveltyFeatures;
};

/**
 * @brief Creates a new Novelty Search instance
 * @tparam P
 * @tparam S
 * @param threshold
 * @param dist
 * @param k
 */
template <typename MS>
NSFeatures<MS>::NSFeatures(unique_ptr<Distance<float>> dist,
                           const int &iterations, const float &threshold,
                           const int &k)
    : NoveltySearch<MS, float>(move(dist), iterations, threshold, k) {}

/**
 * @brief Performs computational work necessary for running the NS
 *  This method creats a combined population using the individuals from the NS
 * archive and the population. The resulting vector contains the features of
 * each individual
 * @tparam MS
 * @param population
 */
template <typename MS>
vector<vars> NSFeatures<MS>::beforeRun(vector<MS> &population) {
    vector<vars> combinedPop;
    combinedPop.reserve(population.size() + this->noveltyArchive.size());
    for (MS &solution : population) {
        combinedPop.push_back(solution.getFeatures());
    }
    combinedPop.insert(combinedPop.end(), this->noveltyFeatures.begin(),
                       this->noveltyFeatures.end());
    return combinedPop;
}

/**
 * @brief Main method of the NS algorithm
 * Computes the sparseness of each individual in the noveltyArchive and
 * population
 * @tparam Problem
 * @tparam MS
 * @tparam T
 * @param population
 * @param problem
 * @return
 */
template <typename MS>
vector<MS> NSFeatures<MS>::run(vector<MS> &population,
                               const Problem<MS> *problem) {
    vector combinedPop = this->beforeRun(population);
    vector<float> spars =
        sparseness(combinedPop, this->distance.get(), this->k);
    for (unsigned int i = 0; i < population.size(); i++) {
        population[i].setDiversity(spars[i]);
    }
    return population;
}

/**
 * @brief Method to insert a new individual into the noveltyArchive of novelty
 * MSs
 * @tparam Problem
 * @tparam MS
 * @param MS
 */
template <typename MS>
void NSFeatures<MS>::insertIntoArchive(const MS &solution) {
    this->noveltyArchive.push_back(solution);
    this->noveltyFeatures.push_back(solution.getFeatures());
}

/**
 * @brief Generates a json object with the relevant information of the class
 *
 * @tparam MS
 * @return json
 */
template <typename MS>
json NSFeatures<MS>::to_json() {
    json data = NoveltySearch<MS, float>::to_json();
    data["name"] = "NSFeatures";

    return data;
}

#endif  // DIGNEA_NOVELTYMEA_H
