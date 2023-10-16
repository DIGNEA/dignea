/**
 * @file Sorter.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2020-12-02
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_SORTBYOBJ_H
#define DIGNEA_SORTBYOBJ_H

#include <dignea/core/Problem.h>
#include <dignea/utilities/Comparator.h>
#include <dignea/utilities/exceptions/EmptyPopulation.h>
#include <dignea/utilities/exceptions/OutOfRange.h>

#include <algorithm>
#include <iostream>
#include <ranges>

using namespace std;

/**
 * @brief Sorts the population of individuals by the given objective according
 * to the problem. The problem must be given to check whether the objective is
 * to Minimize or Maximize.
 *
 * @tparam S
 * @param population
 * @param objNumber
 * @param problem
 */
template <typename S>
void sortByObj(vector<S> &population, const int &objNumber,
               const Problem<S> *problem) {
    if (population.empty()) {
        std::string where = "sortByObj with obj = " + to_string(objNumber);
        throw(EmptyPopulation(where));
    }
    if (objNumber >= population[0].getNumberOfObjs()) {
        std::string where =
            "sortByObj obj = " + to_string(objNumber) + " but only " +
            to_string(population[0].getNumberOfObjs()) + " available";
        throw(OutOfRange(where));
    }
    // Funcion lambda que vamos a emplear para ordenar la poblacion
    // Recibe dos individuos como parametros y devuelve un booleano
    // Tiene en cuenta el tipo de objetivo que estamos considerando: Min or Max.
    // Ademas capturamos el contexto para poder acceder a objectiveNumber
    auto comparator = [&](S &firstInd, S &secondInd) -> bool {
        return (problem->getOptimizationDirection(objNumber) == Minimize)
                   ? (firstInd.getObjAt(objNumber) <
                      secondInd.getObjAt(objNumber))
                   : (firstInd.getObjAt(objNumber) >
                      secondInd.getObjAt(objNumber));
    };
    sort(population.begin(), population.end(), comparator);
}

/**
 * @brief Sorts the population by fitness in descending order.
 *
 * @tparam S
 * @param population
 */
template <typename S>
void sortByFitness(vector<S> &population) {
    if (population.empty()) {
        std::string where = "sortByFitness";
        throw(EmptyPopulation(where));
    }
    auto comparison = [&](S &firstInd, S &secondInd) -> bool {
        return firstInd.getFitness() > secondInd.getFitness();
    };
    sort(population.begin(), population.end(), comparison);
}

/**
 * @brief Sorts the population by their crow distance value in descending order.
 *
 * @tparam S
 * @param population
 */
template <typename S>
void sortByCrowDistance(vector<S> &population) {
    if (population.empty()) {
        std::string where = "sortByFitness";
        throw(EmptyPopulation(where));
    }

    stable_sort(population.begin(), population.end(),
                [](const S &firstInd, const S &secondInd) {
                    return firstInd.getCrowDistance() >
                           secondInd.getCrowDistance();
                });
}

/**
 * @brief
 *
 * @tparam S
 * @param population
 * @param problem
 */
template <typename S>
void crowOrder(vector<S> &population, const Problem<S> *problem) {
    if (population.empty()) {
        std::string where = "crowOrder";
        throw(EmptyPopulation(where));
    }
    if (population[0].getNumberOfObjs() < 2) {
        std::string where = "crowOrder less than two objectives ";
        throw(OutOfRange(where));
    }
    if (population.size() == 1) {
        return;
    }
    double difference = 0;
    // Reset the distance before computing the crowding distance
    for (S &solution : population) {
        solution.setCrowDistance(0.0);
    }
    // We calculate the distance for each objective
    for (int i = 0; i < problem->getNumberOfObjs(); i++) {
        sortByObj(population, i, problem);
        auto size = population.size() - 1;
        population[0].setCrowDistance(std::numeric_limits<float>::max());
        population[size].setCrowDistance(std::numeric_limits<float>::max());

        // Maximum and minimum values of the mth objective function
        // This represents fmaxm fminm from the paper
        difference = population[size].getObjAt(i) - population[0].getObjAt(i);
        if (difference == 0) {
            difference = 1;  // For safe division
        }
        for (unsigned j = 1; j < size; j++) {
            population[j].setCrowDistance(((population[j + 1].getObjAt(i) -
                                            population[j - 1].getObjAt(i)) /
                                           difference) +
                                          population[j].getCrowDistance());
        }
    }
    sortByCrowDistance(population);
}

/**
 * @brief Fast Non Dominated Sorting operator.
 * Sorts the population in different fronts
 * Used in NSGA-II
 *
 * @tparam S
 * @param notClassified
 * @return vector<vector<S>>
 */
template <class S>
vector<vector<S>> fastNonDominatedSort(vector<S> &population,
                                       const Problem<S> *problem) {
    auto popSize = population.size();
    map<int, vector<int>> idxFront;
    map<int, vector<int>> dominates;
    map<int, int> dominatedBy;
    // Initialize the maps
    for (unsigned i = 0; i < popSize; i++) {
        dominates.insert({i, vector<int>()});
        dominatedBy.insert({i, 0});
        population[i].setRank(-1);
    }

    int classified = 0;
    // Calculates Front # 1
    idxFront.insert({0, vector<int>()});
    for (unsigned p = 0; p < popSize; p++) {
        for (unsigned q = p + 1; q < popSize; q++) {
            auto dominated =
                dominanceTest(population[p], population[q], problem);
            if (dominated == FIRST_DOMINATES) {
                dominates[p].push_back(q);
                dominatedBy[q]++;
            } else if (dominated == SECOND_DOMINATES) {
                dominates[q].push_back(p);
                dominatedBy[p]++;
            }
        }
        if (dominatedBy[p] == 0) {
            population[p].setRank(0);
            idxFront[0].push_back(p);
            classified++;
        }
    }
    int index = 0;
    int rank = 1;
    // Now we loop the idxFront
    while (idxFront[index].size() != 0) {
        vector<int> qIdxFront;
        for (unsigned p = 0; p < idxFront[index].size(); p++) {
            for (unsigned q = 0; q < dominates[p].size(); q++) {
                dominatedBy[dominates[p][q]]--;
                if (dominatedBy[dominates[p][q]] == 0) {
                    population[dominates[p][q]].setRank(rank);
                    qIdxFront.push_back(dominates[p][q]);
                    classified++;
                }
            }
        }
        index++;
        rank++;
        idxFront.insert({index, qIdxFront});
    }
    if (classified < (int)popSize) {
        idxFront.insert({index, vector<int>()});
        for (unsigned i = 0; i < popSize; i++) {
            if (population[i].getRank() == -1) {
                population[i].setRank(rank);
                idxFront[index].push_back(i);
                classified++;
            }
        }
    }
    // Just in case the last front was empty
    if (idxFront[index].empty()) {
        idxFront.erase(idxFront.find(index));
    }
    vector<vector<S>> fronts;
    for (auto const &[key, value] : idxFront) {
        vector<S> front;
        std::for_each(value.cbegin(), value.cend(),
                      [&](const int &i) { front.push_back(population[i]); });

        fronts.push_back(front);
    }
    return fronts;
}

#endif  // DIGNEA_SORTBYOBJ_H
