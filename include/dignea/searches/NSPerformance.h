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

/**
 * @brief Class to represent the Novelty Search Algorithm
 * This specialization is exclusively for EIG because it uses
 * the vector of performance of each algorithm over the generated instance to
 * compute novelty
 *
 * @tparam
 */
template <typename S>
class NSPerformance : public NoveltySearch<S> {
   public:
    NSPerformance() = default;

    explicit NSPerformance(unique_ptr<Distance<float>> dist,
                           const float &threshold = 2000,
                           const float &finalThresh = 2000, const int &k = 15);

    virtual ~NSPerformance() = default;

    virtual json to_json() override;

   protected:
    virtual vector<Descriptor> beforeRun(const vector<S> &population) override;

    virtual vector<Descriptor> beforeCmpFinals(
        const vector<S> &population) override;

    virtual void insertFinal(const S &solution);
};

/**
 * @brief Creates a new Novelty Search instance
 * @tparam P
 * @tparam S
 * @param threshold
 * @param dist
 * @param k
 */
template <typename S>
NSPerformance<S>::NSPerformance(unique_ptr<Distance<float>> dist,
                                const float &threshold,
                                const float &finalThresh, const int &k)
    : NoveltySearch<S>(move(dist), threshold, finalThresh, k) {}

/**
 * @brief PerforS computational work necessary for running the NS
 *  This method creats a combined population using the individuals from the NS
 * archive and the population. The resulting vector contains the average
 * performance of each algorithm for each individual
 * @tparam S
 * @param population
 */
template <typename S>
vector<Descriptor> NSPerformance<S>::beforeRun(const vector<S> &population) {
    vector<Descriptor> combinedPop;
    combinedPop.reserve(population.size() + this->noveltyArchive.size());
    for (const S &solution : population) {
        combinedPop.push_back(solution.getAvgPortFitness());
    }
    for (const S &solution : this->noveltyArchive) {
        combinedPop.push_back(solution.getAvgPortFitness());
    }
    // combinedPop.insert(combinedPop.end(), this->noveltyArchive.begin(),
    //                    this->noveltyArchive.end(),
    //                    [](const S &s) { return s.getAvgPortFitness(); });
    return combinedPop;
}

template <typename S>
vector<Descriptor> NSPerformance<S>::beforeCmpFinals(
    const vector<S> &population) {
    vector<Descriptor> descriptors;
    descriptors.reserve(population.size());
    for (const S &solution : population) {
        descriptors.push_back(solution.getAvgPortFitness());
    }
    return descriptors;
}

/**
 * @brief Method to insert a new individual into the noveltyArchive of novelty
 * Ss
 * @tparam Problem
 * @tparam S
 * @param S
 */
template <typename S>
void NSPerformance<S>::insertFinal(const S &solution) {
    this->finalSs.push_back(solution);
    this->finalSsDesc.push_back(solution.getAvgPortFitness());
}

/**
 * @brief Generates a json object with the relevant information of the class
 *
 * @tparam S
 * @return json
 */
template <typename S>
json NSPerformance<S>::to_json() {
    json data = NoveltySearch<S>::to_json();
    data["name"] = "NSPerformance";

    return data;
}

#endif
