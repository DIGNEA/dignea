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
#include <type_traits>

static const int FIRST_BEST = -1;
static const int SECOND_BEST = 1;
static const int TIE = 0;

static const int FIRST_DOMINATES = -1;
static const int SECOND_DOMINATES = 1;
static const int NON_DOMINANCES_EQUALS = 2;
static const int NON_DOMINANCES_NOT_EQUALS = 3;

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

/**
 * @brief Test whether an individual dominates another.
 * A individual dominates another if all their objectives are "better"
 * (considering maximisation or minimisation) or equal and at least one of them
 * is strictly better.
 *
 * @tparam S
 * @param ind1
 * @param ind2
 * @param problem
 * @return int
 */
template <typename S>
int dominanceTest(const S &ind1, const S &ind2, const Problem<S> *problem) {
    if (ind1.getNumberOfObjs() < 2 || ind2.getNumberOfObjs() < 2) {
        std::string where =
            "Comparator::dominanceTest() with less than two objectices";
        throw(std::runtime_error(where));
    }
    if (ind1.getNumberOfObjs() != ind2.getNumberOfObjs()) {
        std::string where =
            "Comparator::dominanceTest() with different number of objectives "
            "each individual";
        throw(std::runtime_error(where));
    }
    int flag;
    auto dominate1{0};
    auto dominate2{0};
    for (int i = 0; i < ind1.getNumberOfObjs(); i++) {
        if (problem->getOptimizationDirection(i) == Maximize) {
            if (ind1.getObjAt(i) > ind2.getObjAt(i))
                flag = -1;
            else if (ind1.getObjAt(i) < ind2.getObjAt(i))
                flag = 1;
            else
                flag = 0;
        } else {  // Objetivo a minimizar
            if (ind1.getObjAt(i) < ind2.getObjAt(i))
                flag = -1;
            else if (ind1.getObjAt(i) > ind2.getObjAt(i))
                flag = 1;
            else
                flag = 0;
        }

        if (flag == -1) dominate1 = 1;
        if (flag == 1) dominate2 = 1;
    }

    if ((dominate1 == 0) && (dominate2 == 0)) {  // iguales
        return NON_DOMINANCES_EQUALS;            // Non dominated solutions
    }
    if ((dominate1 == 1) && (dominate2 == 1)) {  // no iguales
        return NON_DOMINANCES_NOT_EQUALS;
    }
    if (dominate1 == 1) {
        return FIRST_DOMINATES;  // solution1 dominates
    }
    return SECOND_DOMINATES;  // solution2 dominates
}

/**
 * @brief Compare whether two vectors are equal within a tolerance.
 *
 * @tparam T
 * @param v1
 * @param v2
 * @return true
 * @return false
 */
template <typename T>
bool areEqual(const vector<T> &v1, const vector<T> &v2) {
    const T tolerance = 0.01;
    if (v1.size() != v2.size()) {
        return false;  // Vectors have different sizes, they cannot be equal
    }

    for (std::size_t i = 0; i < v1.size(); ++i) {
        if (std::abs(v1[i] - v2[i]) > tolerance) {
            return false;  // Difference between elements exceeds tolerance,
                           // vectors are not equal
        }
    }
    return true;  // All corresponding elements are within tolerance, vectors
                  // are considered equal
}

/**
 * @brief Compare whether float/double values are equal within a tolerance.
 *
 * @tparam T
 * @param v1
 * @param v2
 * @return true
 * @return false
 */
template <typename T>
bool areEqual(const T &a, const T &b) {
    const T tolerance = 0.0001f;

    return (std::abs(a - b) < tolerance * std::max(std::abs(a), std::abs(b)));
}

#endif  //__COMPARATOR_H__
