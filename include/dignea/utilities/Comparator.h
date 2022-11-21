/**
 * @file Comparator.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2020-12-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __COMPARATOR_H__
#define __COMPARATOR_H__

#include <dignea/core/Problem.h>
#include <dignea/core/Solution.h>
#include <dignea/utilities/exceptions/OutOfRange.h>

#include <numeric>

static const int FIRST_BEST = -1;
static const int SECOND_BEST = 1;
static const int TIE = 0;

/**
 * @brief Compares two individuals by their fitness
 * @tparam S
 * @param ind1
 * @param ind2
 * @return FIRST_BEST, SECOND_BEST or TIE.
 */
template <typename S>
int cmpByFitness(const S &ind1, const S &ind2) {
    double fitness1 = ind1.getFitness();
    double fitness2 = ind2.getFitness();
    if (fitness1 > fitness2) {
        return FIRST_BEST;
    }
    if (fitness1 < fitness2) {
        return SECOND_BEST;
    }
    return TIE;
}

/**
 * @brief Compares two individuals by the objective nObj. The problem to solve
 * must be given to check whether the objective is to be Minimize or Maximize.
 *
 * @tparam S
 * @param ind1
 * @param ind2
 * @param nObj
 * @param problem
 * @return int
 */
template <typename S>
int cmpByObj(const S &ind1, const S &ind2, const int &nObj,
             const Problem<S> *problem) {
    if (ind1.getNumberOfObjs() < nObj || ind2.getNumberOfObjs() < nObj) {
        std::string where =
            "Comparator::cmpByObj() with obj = " + to_string(nObj);
        throw(OutOfRange(where));
    }
    double obj1 = ind1.getObjAt(nObj);
    double obj2 = ind2.getObjAt(nObj);
    // Diferenciamos los casos de minimizacion y maximizacion de objetivos
    if (problem->getOptimizationDirection(nObj) == Minimize) {
        if (obj1 < obj2) {
            return FIRST_BEST;
        }
        if (obj1 > obj2) {
            return SECOND_BEST;
        }
    } else {
        if (obj1 > obj2) {
            return FIRST_BEST;
        }
        if (obj1 < obj2) {
            return SECOND_BEST;
        }
    }
    return TIE;
}

/**
 * @brief Checks whether a solution is improved by a new one. A solution (with
 constraints) is said to improve an existing one if:
 * - Share the same constraint coefficient by have a better fitness.
 * - Have a constraint coefficient lower than the previous solution.

 * @tparam Problem
 * @tparam S
 * @param ind1
 * @param ind2
 * @return FIRST_BEST if ind1 is the best solution, SECOND_BEST otherwise. If
 the constraints are the same they are compared by fitness using cmpByFitness.
 */
template <typename S>
int improves(const S &ind1, const S &ind2) {
    if (ind1.getNumberOfCons() != ind2.getNumberOfCons()) {
        std::string where = "Comparator::improves() different constraints";
        throw(OutOfRange(where));
    }
    vector ind1Const = ind1.getConstraints();
    vector ind2Const = ind2.getConstraints();
    float ind1Penalty =
        std::accumulate(ind1Const.begin(), ind1Const.end(), 0.0f);
    float ind2Penalty =
        std::accumulate(ind2Const.begin(), ind2Const.end(), 0.0f);

    if (ind1Penalty < ind2Penalty) {
        return FIRST_BEST;
    } else if (ind2Penalty < ind1Penalty) {
        return SECOND_BEST;
    } else {
        // Comprobamos el fitness
        return cmpByFitness(ind1, ind2);
    }
}

#endif  //__COMPARATOR_H__
