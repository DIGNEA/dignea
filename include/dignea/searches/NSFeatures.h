//
// Created by amarrero on 28/6/21.
//

#ifndef DIGNEA_NSFEATURES_H
#define DIGNEA_NSFEATURES_H

#include <dignea/distances/Distance.h>
#include <dignea/generator/AbstractInstance.h>
#include <dignea/searches/NoveltySearch.h>
#include <dignea/searches/Search.h>
#include <dignea/utilities/KNN.h>
#include <dignea/utilities/NSWarmUp.h>
#include <dignea/utilities/Sorter.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

#include "NumCpp.hpp"

using namespace std;
using json = nlohmann::json;

/**
 * @brief Class to represent the Novelty Search Algorithm
 * This specialization is exclusively for EIG because it uses
 * the vector of features of each generated instance to compute novelty
 *
 * @tparam S --> AbstractInstance type
 */
template <typename S>
class NSFeatures : public NoveltySearch<S> {
   public:
    NSFeatures() = default;

    explicit NSFeatures(unique_ptr<Distance<float>> dist,
                        const float &threshold = 2000,
                        const float &finalThresh = 2000, const int &k = 15,
                        bool warmUp = false);

    virtual ~NSFeatures() = default;

    virtual json to_json() override;

   protected:
    virtual vector<Descriptor> beforeRun(const vector<S> &population) override;

    virtual vector<Descriptor> beforeCmpFinals(
        const vector<S> &population) override;

    virtual void insertFinal(const S &solution) override;

   protected:
    bool warmed;
};

/**
 * @brief Construct a new NSFeatures<S>::NSFeatures object
 *  If warmUp is set, the NS archive is initialised with random points from
 * the KP space
 * @tparam S
 * @param dist
 * @param iterations
 * @param threshold
 * @param k
 * @param warmUp
 */
template <typename S>
NSFeatures<S>::NSFeatures(unique_ptr<Distance<float>> dist,
                          const float &threshold, const float &finalThresh,
                          const int &k, bool warmUp)
    : NoveltySearch<S>(move(dist), threshold, finalThresh, k), warmed(warmUp) {
    // if (warmUp) {
    //     auto seed = 42;
    //     nc::random::seed(seed);
    //     auto indexes =
    //         nc::random::randInt(nc::Shape(1, 50), (long unsigned int)0,
    //                             NSWarmUp::warmUpData.size());

    //     for (auto ix : indexes) {
    //         this->noveltyFeatures.push_back(NSWarmUp::warmUpData[ix]);
    //     }
    // }
}

/**
 * @brief Performs computational work necessary for running the NS
 *  This method creates a combined population using the individuals from the NS
 * archive and the population. The resulting vector contains the features of
 * each individual
 * @tparam S
 * @param population
 */
template <typename S>
vector<Descriptor> NSFeatures<S>::beforeRun(const vector<S> &population) {
    vector<Descriptor> combinedPop;
    combinedPop.reserve(population.size() + this->noveltyArchive.size());
    for (const S &solution : population) {
        combinedPop.push_back(solution.getFeatures());
    }
    for (const S &solution : this->noveltyArchive) {
        combinedPop.push_back(solution.getFeatures());
    }
    // combinedPop.insert(combinedPop.end(), this->noveltyArchive.begin(),
    //                    this->noveltyArchive.end(),
    //                    [](const S &s) { return s.getFeatures(); });
    return combinedPop;
}

template <typename S>
vector<Descriptor> NSFeatures<S>::beforeCmpFinals(const vector<S> &population) {
    vector<Descriptor> descriptors;
    descriptors.reserve(population.size());
    for (const S &solution : population) {
        descriptors.push_back(solution.getFeatures());
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
void NSFeatures<S>::insertFinal(const S &solution) {
    this->finalSs.push_back(solution);
    this->finalSsDesc.push_back(solution.getFeatures());
}

/**
 * @brief Generates a json object with the relevant information of the class
 *
 * @tparam S
 * @return json
 */
template <typename S>
json NSFeatures<S>::to_json() {
    json data = NoveltySearch<S>::to_json();
    data["name"] = "NSFeatures";
    data["warm-up"] = this->warmed;
    return data;
}

#endif  // DIGNEA_NOVELTYEIG_H
