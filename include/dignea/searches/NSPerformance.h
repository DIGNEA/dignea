//
// Created by amarrero on 28/6/21.
//

#ifndef DIGNEA_NSPERFORMANCE_H
#define DIGNEA_NSPERFORMANCE_H

#include <dignea/distances/Distance.h>
#include <dignea/generator/AbstractInstance.h>
#include <dignea/searches/NSFeatures.h>
#include <dignea/utilities/KNN.h>
#include <dignea/utilities/Sorter.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

using namespace std;
using json = nlohmann::json;

using vars = vector<float>;

/**
 * @brief Class to represent the Novelty Search Algorithm
 * This specialization is exclusively for EIG because it uses
 * the vector of performance of each algorithm over the generated instance to
 * compute novelty
 *
 * @tparam MS --> AbstractInstance type
 */
template <typename MS>
class NSPerformance : public NSFeatures<MS> {
   public:
    NSPerformance() = default;

    explicit NSPerformance(unique_ptr<Distance<float>> dist,
                           const int &iterations, const float &threshold = 2000,
                           const int &k = 15);

    virtual ~NSPerformance() = default;

    void insertIntoArchive(const MS &solution) override;

    virtual json to_json() override;

   protected:
    virtual vector<vars> beforeRun(vector<MS> &population) override;
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
NSPerformance<MS>::NSPerformance(unique_ptr<Distance<float>> dist,
                                 const int &iterations, const float &threshold,
                                 const int &k)
    : NSFeatures<MS>(move(dist), iterations, threshold, k) {}

/**
 * @brief Performs computational work necessary for running the NS
 *  This method creats a combined population using the individuals from the NS
 * archive and the population. The resulting vector contains the average
 * performance of each algorithm for each individual
 * @tparam MS
 * @param population
 */
template <typename MS>
vector<vars> NSPerformance<MS>::beforeRun(vector<MS> &population) {
    vector<vars> combinedPop;
    combinedPop.reserve(population.size() + this->noveltyArchive.size());
    for (MS &solution : population) {
        combinedPop.push_back(solution.getAvgPortFitness());
    }
    combinedPop.insert(combinedPop.end(), this->noveltyFeatures.begin(),
                       this->noveltyFeatures.end());
    return combinedPop;
}

/**
 * @brief Method to insert a new individual into the noveltyArchive of novelty
 * MSs
 * @tparam Problem
 * @tparam MS
 * @param MS
 */
template <typename MS>
void NSPerformance<MS>::insertIntoArchive(const MS &solution) {
    this->noveltyArchive.push_back(solution);
    this->noveltyFeatures.push_back(solution.getAvgPortFitness());
}

/**
 * @brief Generates a json object with the relevant information of the class
 *
 * @tparam MS
 * @return json
 */
template <typename MS>
json NSPerformance<MS>::to_json() {
    json data = NSFeatures<MS>::to_json();
    data["name"] = "NSPerformance";

    return data;
}

#endif
