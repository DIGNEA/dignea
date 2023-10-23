//
// Created by amarrero on 18/6/21.
//
#ifndef DIGNEA_NOVELTYSEARCH_H
#define DIGNEA_NOVELTYSEARCH_H

#include <dignea/distances/Distance.h>
#include <dignea/generator/AbstractInstance.h>
#include <dignea/utilities/Comparator.h>
#include <dignea/utilities/KNN.h>
#include <dignea/utilities/Sorter.h>
#include <dignea/utilities/exceptions/EmptyPopulation.h>
#include <fmt/core.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

#include "Search.h"

using namespace std;
using json = nlohmann::json;

typedef vector<float> Descriptor;
/**
 * @brief Cast a vector of one type to another.
 *
 * @tparam F
 * @tparam T
 * @param from
 * @return vector<T>
 */

template <typename F>
Descriptor castDescriptor(const std::vector<F> &from) {
    Descriptor castedVector;
    castedVector.reserve(from.size());
    std::transform(from.begin(), from.end(), std::back_inserter(castedVector),
                   [](F value) { return static_cast<float>(value); });
    return castedVector;
}

/**
 * @brief Class to represent the Novelty Search Algorithm
 *
 * @tparam P
 * @tparam S
 */
template <typename S>
class NoveltySearch : public Search<S> {
   public:
    NoveltySearch() = default;

    explicit NoveltySearch(unique_ptr<Distance<float>> dist,
                           const float &threshold = 2000,
                           const float &finalThresh = 0.0001,
                           const int &k = 15);

    virtual ~NoveltySearch() = default;

    Front<S> getResults() override;

    vector<S> run(vector<S> &population, const Problem<S> *problem) override;

    virtual void cmpFinals(vector<S> &population,
                           const Problem<S> *problem = nullptr);

    virtual void insertIntoArchive(const S &solution);

    inline const vector<S> &getArchive() const { return this->noveltyArchive; }

    virtual json to_json();

    float getThreshold() const { return this->threshold; }

    float getK() const { return this->k; }

    float getFinalThresh() const { return this->finalSThreshold; }

   protected:
    virtual vector<Descriptor> beforeRun(const vector<S> &population);

    virtual vector<Descriptor> beforeCmpFinals(const vector<S> &population);

    virtual void insertFinal(const S &solution);

   protected:
    unique_ptr<Distance<float>> distance;
    vector<S> noveltyArchive;
    vector<S> finalSs;
    vector<Descriptor> finalSsDesc;

    float threshold;
    float finalSThreshold;
    int k;
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
NoveltySearch<S>::NoveltySearch(unique_ptr<Distance<float>> dist,
                                const float &threshold,
                                const float &finalThresh, const int &k)
    : distance(move(dist)),
      threshold(threshold),
      finalSThreshold(finalThresh),
      k(k) {}

/**
 * @brief Performs computational work necessary for running the NS
 *  This method creates a combined population using the individuals from the NS
 * archive and the population. The resulting vector contains the features of
 * each individual
 * @tparam MS
 * @param population
 */
template <typename S>
vector<Descriptor> NoveltySearch<S>::beforeRun(const vector<S> &population) {
    vector<Descriptor> combinedPop;
    combinedPop.reserve(population.size() + this->noveltyArchive.size());
    for (const S &solution : population) {
        vector vars = solution.getVariables();
        combinedPop.push_back(castDescriptor(vars));
    }
    for (const S &solution : noveltyArchive) {
        vector vars = solution.getVariables();
        combinedPop.push_back(castDescriptor(vars));
    }
    // combinedPop.insert(combinedPop.end(), this->noveltyArchive.begin(),
    //                    this->noveltyArchive.end(), [](const S &s) {
    //                        vector vars = s.getVariables();
    //                        return castVector(vars);
    //                    });
    return combinedPop;
}

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
template <typename S>
vector<S> NoveltySearch<S>::run(vector<S> &population,
                                const Problem<S> *problem) {
    vector<Descriptor> combinedPopulation = beforeRun(population);
    // vector<S> combinedPop = population;
    // combinedPop.insert(combinedPop.end(), noveltyArchive.begin(),
    //                    noveltyArchive.end());
    vector spars = sparseness(combinedPopulation, this->distance.get(), k);
    auto momea = false;
    if (population[0].getNumberOfObjs() == 2) {
        // Running with MOEIG, we set diversity as objective 2
        momea = true;
    }
    for (unsigned int i = 0; i < population.size(); i++) {
        population[i].setDiversity(spars[i]);
        if (momea) {
            population[i].setObjAt(1, spars[i]);
        }
    }
    return population;
    // vector spars = sparseness(combinedPop, distance.get(), k);
    // for (unsigned int i = 0; i < population.size(); i++) {
    //     population[i].setDiversity(spars[i]);
    // }
    // return population;
}

template <typename S>
vector<Descriptor> NoveltySearch<S>::beforeCmpFinals(
    const vector<S> &population) {
    vector<Descriptor> descriptors;
    descriptors.reserve(population.size());
    for (const S &solution : population) {
        vector vars = solution.getVariables();
        descriptors.push_back(castDescriptor(vars));
    }
    return descriptors;
}

/**
 * @brief Compares the individuals in the population against the neighbors
 * inside the archive of final Ss. If the score is good enough the individual
 * will be included inside the archive of final solutions
 * @tparam Problem
 * @tparam S
 * @tparam T
 * @param population
 */
template <typename S>
void NoveltySearch<S>::cmpFinals(vector<S> &population,
                                 const Problem<S> *problem) {
    if (population.size() == 0) {
        std::string where = "NoveltySearch::cmpFinals";
        throw EmptyPopulation(where);
    }
#ifdef DEBUG
    std::cout << fmt::format("{:=^120}", " Including inds. final solution set ")
              << std::endl;
    std::cout << fmt::format("{:=^120}", "") << std::endl;
    std::cout << fmt::format("{:^30}\t{:^30}\t{:^20}({})\t{:^20}", "Sparseness",
                             "Biased Fitness", "Greater than threshold",
                             this->finalSThreshold, "Archive len()")
              << std::endl;
    std::cout << fmt::format("{:=^120}", "") << std::endl;
#endif

    if (population[0].getNumberOfObjs() == 1) {
        // Includes fittest feasible individual if the archive is empty
        if (finalSs.empty()) {
            sortByFitness(population);
            for (S &sol : population) {
                if (sol.getFitness() > 0.0) {
                    this->insertFinal(sol);
                    break;
                }
            }

        } else {
            // Includes feasible individuals with spars > threshold
            // Here we require the individual to be feasible in the performance;
            // i.e., target algorithm performs better than the others
            // and to have a diversity value greater than the predefined
            // threshold
            vector descriptors = beforeCmpFinals(population);
            for (unsigned int i = 0; i < descriptors.size(); i++) {
                float spars = sparseness(descriptors[i], this->finalSsDesc,
                                         distance.get(), 1);
                if ((population[i].getBiasedFitness() > 0.0) &&
                    (spars > this->finalSThreshold)) {
                    this->insertFinal(population[i]);
                }
            }

#ifdef DEBUG
            std::cout << fmt::format(
                             "{:>20}\t{:>30}\t{:>20}\t{:>20}", spars,
                             solution.getBiasedFitness(),
                             (spars > this->finalSThreshold ? "Yes" : "No"),
                             finalSs.size())
                      << std::endl;

#endif
        }
    } else {
        if (!problem) {
            string where =
                "problem is nullptr in NoveltySearch::cmpFinals. When using "
                "MOEIG cmpFinals must receive the problem as well";
            throw(std::runtime_error(where));
        }
        if (finalSs.empty()) {
            sortByObj(population, 0, problem);
            for (S &sol : population) {
                if (sol.getObjAt(0) > 0.0) {
                    this->insertFinal(sol);
                    break;
                }
            }
        } else {
            // Includes feasible individuals with spars > threshold
            // Here we require the individual to be feasible in the performance;
            // We get the descriptors of all the instances at once
            vector descriptors = beforeCmpFinals(population);
            for (unsigned int i = 0; i < descriptors.size(); i++) {
                float spars = sparseness(descriptors[i], this->finalSsDesc,
                                         distance.get(), 1);
                if ((population[i].getObjAt(0) > 0.0) &&
                    (spars > this->finalSThreshold)) {
                    this->insertFinal(population[i]);
                }
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
template <typename S>
Front<S> NoveltySearch<S>::getResults() {
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
template <typename S>
void NoveltySearch<S>::insertIntoArchive(const S &solution) {
    this->noveltyArchive.push_back(solution);
}

/**
 * @brief Method to insert a new individual into the final set of solutions
 * The default behaviour is to get all the variables of the solution
 * @tparam Problem
 * @tparam S
 * @tparam T
 * @param S
 */
template <typename S>
void NoveltySearch<S>::insertFinal(const S &solution) {
    this->finalSs.push_back(solution);
    this->finalSsDesc.push_back(castDescriptor(solution.getVariables()));
}

template <typename S>
json NoveltySearch<S>::to_json() {
    json data;
    data["name"] = "Novelty Search";
    data["k"] = this->k;
    data["threshold"] = this->threshold;
    return data;
}

#endif  // DIGNEA_NOVELTYSEARCH_H
