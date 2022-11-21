//
// Created by amarrero on 17/10/19.
//

#ifndef DIGNEA_PROBLEM_H
#define DIGNEA_PROBLEM_H

#include <dignea/core/Solution.h>
#include <dignea/utilities/random/ParallelPRNG.h>

#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <vector>

using namespace std;
using json = nlohmann::json;

enum optimisationDirection { Minimize, Maximize };

/**
 * @brief Class to represent a Problem in the tool. It includes the basic
 * information for a problem a few methods that must be implemented in the
 * subclasses
 *
 * @tparam S Type of individual in the population
 */
template <class S>
class Problem {
   public:
    Problem();

    Problem(const int &numberOfVars, const int &numberOfObjectives,
            const int &nCons);

    Problem(const Problem *copy);

    virtual ~Problem() = default;

   public:
    /**
     * @brief Method to evaluate whether a solution is feasible or not. This
     * method must be implemented in the subclasses.
     *
     * @param sol Solution (S) to evaluate
     * @return true if the solution is feasible
     * @return false otherwise
     */
    virtual bool evaluateConstraints(S &sol) const = 0;

    /**
     * @brief Method to evaluate a solution using the proper problem
     * formulation. This method must be implemented in the subclasses.
     *
     * @param sol Solution (S) to evaluate
     */
    virtual void evaluate(S &sol) const = 0;

    /**
     * @brief Creates a new solution for the problem. Usually uses problem
     * information and generates a solution randomly. This method must be
     * implemented in the subclasses.
     *
     * @return S Solution (S)
     */
    virtual S createSolution() const = 0;

    virtual S createSolution(ParallelPRNG &engine) const = 0;

    /**
     * @brief Returns the optimization direction for each objective in the
     * problem. It returns Minimize or Maximize accordingly. This method must be
     * implemented in the subclasses.
     *
     * @param i Dimension to check
     * @return Minimize or Maximize constants.
     */
    virtual int getOptimizationDirection(const int i) const = 0;

    /**
     * @brief Returns the upper bound to the problem at dimension i.
     * This method must be implemented in the subclasses.
     *
     * @param i Dimension to check
     * @return float
     */
    virtual float getUpperLimit(const int i) const = 0;

    /**
     * @brief Returns the lower bound to the problem at dimension i. This method
     * must be implemented in the subclasses.
     *
     * @param i
     * @return float
     */
    virtual float getLowerLimit(const int i) const = 0;

    /**
     * @brief Returns the name of the problem. This method must be implemented
     * in the subclasses.
     *
     * @return string
     */
    virtual string getName() const = 0;

    /**
     * @brief Method to read problem information from the given path. This
     * method must be implemented in the subclasses.
     *
     * @param path File path to get the information
     */
    virtual void readData(const std::string &path) = 0;

    virtual json to_json() const;

   public:
    /**
     * @brief Get the number of variables (dimension) of the problem
     *
     * @return int
     */
    inline int getNumberOfVars() const { return numberOfVars; };
    /**
     * @brief Sets the number of variables (dimension) of the problem
     *
     * @param nVars
     */
    inline void setNumberOfVars(int nVars) { numberOfVars = nVars; };

    /**
     * @brief Get the number of objectives of the problem
     *
     * @return int
     */
    inline int getNumberOfObjs() const { return numberOfObjs; };

    /**
     * @brief Sets the number of objectives to the problem
     *
     * @param nObjs
     */
    inline void setNumberOfObjs(int nObjs) { numberOfObjs = nObjs; };

    /**
     * @brief Get the number the constraint of the problem
     *
     * @return int
     */
    inline int getNumberOfCons() const { return numberOfCons; };
    /**
     * @brief Sets the number of constraint of the problem
     *
     * @param nCons
     */
    inline void setNumberOfCons(int nCons) { numberOfCons = nCons; };

   protected:
    int numberOfVars; /*!<  Number of variables of the solution (dimension). */
    int numberOfObjs; /*!<  Number of objectives of the solution. */
    int numberOfCons; /*!<  Number of constraints of the solution. */
};

/**
 * @brief Creates a new Problem with all parameters to zero.
 *
 * @tparam S
 */
template <class S>
Problem<S>::Problem() : numberOfVars(0), numberOfObjs(0), numberOfCons(0) {}

/**
 * @brief Construct a new Problem object from the Problem ind.
 * Copies the information to this new problem instance.
 * @tparam S
 * @param ind
 */
template <class S>
Problem<S>::Problem::Problem(const Problem *ind) {
    numberOfVars = ind->numberOfVars;
    numberOfObjs = ind->numberOfObjs;
}

/**
 * @brief Construct a new Problem with all the given parameters
 *
 * @tparam S
 * @param nVars  Number of variables in the problem
 * @param nObjs  Number of objectives in the problem
 * @param nCons  Number of constraint in the problem
 */
template <class S>
Problem<S>::Problem(const int &nVars, const int &nObjs, const int &nCons) {
    this->numberOfVars = nVars;
    this->numberOfObjs = nObjs;
    this->numberOfCons = nCons;
}

/**
 * @brief Creates a JSON object with the information of the Problem
 * @tparam S
 * @return a json representation of the problem
 */
template <class S>
json Problem<S>::to_json() const {
    json data;
    data["name"] = this->getName();
    data["num_objs"] = this->numberOfObjs;
    data["num_vars"] = this->numberOfVars;
    data["num_cons"] = this->numberOfCons;
    data["up_limit"] = this->getUpperLimit(0);
    data["low_limit"] = this->getLowerLimit(0);
    return data;
}

#endif  // DIGNEA_PROBLEM_H
