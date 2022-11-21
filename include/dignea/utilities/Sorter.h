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

#include <iostream>

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

#endif  // DIGNEA_SORTBYOBJ_H
