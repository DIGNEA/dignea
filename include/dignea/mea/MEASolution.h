/**
 * @file MEASolution.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 8/3/21
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __MEA_SOLUTION_H__
#define __MEA_SOLUTION_H__

#include <dignea/core/Front.h>
#include <dignea/core/Solution.h>

#include <algorithm>
#include <vector>

using namespace std;

/**
 * @brief Solution for a Instance Generation Problem used in the MEA. This is a
 * solution for MEAProblem.
 * @tparam V Variable type for the solution representation
 * @tparam O Objective type for the fitness
 */
template <typename V, typename O>
class MEASolution : public Solution<V, O> {
   public:
    MEASolution();

    explicit MEASolution(const int &nVars, const int &nObjs = 1,
                         const int &nCons = 0);

    MEASolution(const MEASolution &solution);

    explicit MEASolution(const MEASolution *solution);

    MEASolution(unique_ptr<MEASolution> &ptr) = delete;

    virtual ~MEASolution() = default;

    MEASolution &operator=(const MEASolution &copy);

    bool operator==(const MEASolution &other) const;

    /**
     * @brief Creates a JSON format representation of the solution.
     * It must be override by the subclasses
     *
     * @return json
     */
    json to_json() const override { return json{}; }

    /**
     * @brief Set the features descriptor of this instance.
     *
     * @param v
     */
    void setFeatures(const vector<float> &v);

    /**
     * @brief Get the features descriptor of this instance
     *
     * @return vector<float> with the features of the solution
     */
    vector<float> getFeatures() const { return features; }

    /**
     * @brief Get the Diversity of the solution.
     *
     * @return float
     */
    float getDiversity() const { return diversity; }

    /**
     * @brief Set the Diversity of the solution. The diversity of the instance
     * is computed using the Novelty Search.
     *
     * @param diversity of type float
     */
    void setDiversity(float diversity) { MEASolution::diversity = diversity; }

    /**
     * @brief Get the Biased Fitness value. The BiasedFitness is the fitness
     * value compute exclusively from the difference in performance between the
     * target and the other algorithms in the portfolio. To get the fitness
     * value computed with the WFF use the getFitness() method.
     *
     * @return float
     */
    float getBiasedFitness() const { return biasedFitness; }

    /**
     * @brief Set the Biased Fitness value. The BiasedFitness is the fitness
     * value compute exclusively from the difference in performance between the
     * target and the other algorithms in the portfolio.
     *
     * @param bFitness
     */
    void setBiasedFitness(float bFitness) {
        MEASolution::biasedFitness = bFitness;
    }

    /**
     * @brief Set the fitness of each algorithm in the porftolio over this
     * instance
     *
     * @param fitness A matrix of fitness. Each row represents an algorithm and
     * each column the fitness of each repetition performed
     */
    void setConfigFitness(const vector<vector<float>> &fitness) {
        this->portFitness = fitness;
    }

    /**
     * @brief Set the average fitness of each algorithm in the porftolio over
     *this instance
     *
     * @param fitness vector of fitness, each value is the average fitness
     * obtained after several reps for each algorithm in the portfolio
     **/
    void setAvgConfigFitness(const vector<float> &fitness) {
        this->avgPortFitness = fitness;
    }

    /**
     * @brief Get the Config Fitness matrix
     *
     * @return vector<vector<float>> A matrix of fitness. Each row represents an
     * algorithm and each column the fitness of each repetition performed
     */
    vector<vector<float>> getPortFitness() const { return portFitness; }

    /**
     * @brief Get the Avg Config Fitness vector
     *
     * @return vector<float> vector of fitness, each value is the average
     * fitness obtained after several reps for each algorithm in the portfolio
     */
    vector<float> getAvgPortFitness() const { return avgPortFitness; }

    /**
     * @brief Not implemented here. The purpose of this method is to generate a
     * string representation of the instance to dump into a
     * file
     *
     * @param os
     */
    virtual void to_instance(std::ostream &os) const {
        os << "Not implemented in MEASolution" << std::endl;
    };

    /**
     * @brief Dumps into the ostream the instance representation
     *
     * @param os
     * @param solution
     * @return std::ostream&
     */
    friend std::ostream &operator<<(std::ostream &os,
                                    MEASolution<V, O> const &solution) {
        solution.to_instance(os);
        return os;
    }

   protected:
    /**
     * @brief Vector of features of the instance. This is problem dependent and
     * must defined in order and computed after each evaluation of the instance.
     *
     */
    vector<float> features;
    /**
     * @brief Individual diversity respect to the rest of the population
     */
    float diversity;
    /**
     * @brief Individual's fitness regarding to the performance of the target
     * algorithm
     */
    float biasedFitness;
    /**
     * @brief Best fitness founds for all the algorithms in the portfolio that
     * has solved the instance
     */
    vector<vector<float>> portFitness;
    /**
     * @brief Average best fitness founds for all the algorithms in the
     * portfolio that has solved the instance in the number of repetitions
     * performed
     */
    vector<float> avgPortFitness;
};

/**
 * @brief Construct a new MEASolution<V, O>::MEASolution object
 *
 * @tparam V Variable type for the solution representation
 * @tparam O Objective type for the fitness
 */
template <typename V, typename O>
MEASolution<V, O>::MEASolution()
    : Solution<V, O>(),
      features({}),
      diversity(0.0),
      biasedFitness(0.0),
      portFitness({}),
      avgPortFitness({}) {}

/**
 * @brief Construct a new MEASolution<V, O>::MEASolution object
 * from the data given in the arguments
 * @tparam V Variable type for the solution representation
 * @tparam O Objective type for the fitness
 * @param nVars Integer representing the dimnesion (number of variables) of the
 * solution
 * @param nObjs Integer representing the number of objectives of the solution
 * @param nCons Integer representing the number of constraints of the solution
 */
template <typename V, typename O>
MEASolution<V, O>::MEASolution(const int &nVars, const int &nObjs,
                               const int &nCons)
    : Solution<V, O>(nVars, nObjs, nCons),
      features({}),
      diversity(0.0),
      biasedFitness(0.0),
      portFitness({}),
      avgPortFitness({}) {}

/**
 * @brief Construct a new MEASolution<V, O>::MEASolution object from the data in
 * the given solution
 *
 * @tparam V Variable type for the solution representation
 * @tparam O Objective type for the fitness
 * @param solution MEASolution to create a copy of
 */
template <typename V, typename O>
MEASolution<V, O>::MEASolution(const MEASolution &solution)
    : Solution<V, O>(solution) {
    this->features = solution.features;
    this->diversity = solution.diversity;
    this->biasedFitness = solution.biasedFitness;
    this->portFitness = solution.portFitness;
    this->avgPortFitness = solution.avgPortFitness;
}

/**
 * @brief Construct a new MEASolution<V, O>::MEASolution object from the data in
 * the given solution
 *
 * @tparam V Variable type for the solution representation
 * @tparam O Objective type for the fitness
 * @param solution pointer to a MEASolution to create a copy of
 */
template <typename V, typename O>
MEASolution<V, O>::MEASolution(const MEASolution *solution)
    : Solution<V, O>(solution) {
    this->features = solution->features;
    this->diversity = solution->diversity;
    this->biasedFitness = solution->biasedFitness;
    this->portFitness = solution->portFitness;
    this->avgPortFitness = solution->avgPortFitness;
}

/**
 * @brief Updates the  features of the  solution
 *
 * @tparam V Variable type for the solution representation
 * @tparam O Objective type for the fitness
 * @param v Vector of float values with the features of the solution
 */
template <typename V, typename O>
void MEASolution<V, O>::setFeatures(const vector<float> &v) {
    this->features = v;
}

/**
 * @brief Copies a solution using the assignment operator
 *
 * @tparam V Variable type for the solution representation
 * @tparam O Objective type for the fitness
 * @param copy MEASolution to create a copy of
 * @return Solution<V, O>&
 */
template <typename V, typename O>
MEASolution<V, O> &MEASolution<V, O>::operator=(const MEASolution &copy) {
    if (this == &copy) {
        return *this;
    }
    this->nVars = copy.nVars;
    this->nObjs = copy.nObjs;
    this->nCons = copy.nCons;
    this->fitness = copy.fitness;
    this->constraintCoeff = copy.constraintCoeff;
    this->variables = copy.variables;
    this->objectives = copy.objectives;
    this->constraints = copy.constraints;
    features = copy.features;
    diversity = copy.diversity;
    biasedFitness = copy.biasedFitness;
    portFitness = copy.portFitness;
    avgPortFitness = copy.avgPortFitness;

    return *this;
}

/**
 * @brief Compares whether two solutions are equal
 *
 * @tparam V Variable type for the solution representation
 * @tparam O Objective type for the fitness
 * @param other MEASolution to compare to
 * @return true
 * @return false
 */
template <typename V, typename O>
bool MEASolution<V, O>::operator==(const MEASolution &other) const {
    if (other.fitness != this->fitness) {
        return false;
    }
    if (other.diversity != this->diversity) {
        return false;
    }
    if (other.biasedFitness != this->biasedFitness) {
        return false;
    }
    if (other.constraintCoeff != this->constraintCoeff) {
        return false;
    }
    if ((this->nVars != other.nVars) || (this->nObjs != other.nObjs) ||
        (this->nCons != other.nCons)) {
        return false;
    }
    for (int i = 0; i < other.nVars; i++) {
        if (other.variables[i] != this->variables[i]) {
            return false;
        }
    }
    for (int i = 0; i < other.nObjs; i++) {
        if (other.objectives[i] != this->objectives[i]) {
            return false;
        }
    }
    for (int i = 0; i < other.nCons; i++) {
        if (other.constraints[i] != this->constraints[i]) {
            return false;
        }
    }
    for (unsigned int i = 0; i < other.features.size(); i++) {
        if (other.features[i] != this->features[i]) {
            return false;
        }
    }
    return true;
}

#endif  // __MEA_SOLUTION_H__