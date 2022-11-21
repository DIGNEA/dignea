/**
 * @file TSP.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-03-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DIGNEA_TSP_H
#define DIGNEA_TSP_H

#include <dignea/core/Problem.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <fstream>
#include <iostream>
#include <utility>

/**
 * @brief Coordinates of each city in the TSP instance
 *
 */
using coords = std::pair<float, float>;

/**
 * @brief Traveling Salesman Problem implemented in dignea. Uses a IntFloat
 * representation where the solutions are integers in a permutation-based
 * approach and the fitness is a float with the cost of a travel
 *
 */
class TSP : public Problem<IntFloatSolution> {
   public:
    /// @brief Creates a default TSP object
    TSP();

    virtual ~TSP() = default;

    /// @brief Creates a TSP object with a predefined number of cities
    /// @param numberOfVars Cities in include in the instance
    explicit TSP(const int &numberOfVars);

    /// @brief Creates a TSP object with a predefined number of cities and the
    /// coordinates for each one
    /// @param numberOfVars
    /// @param coordinates
    explicit TSP(const int &numberOfVars, const vector<coords> &coordinates);

    /// @brief Creates a TSP object which loads the data from the filesystem.
    /// @param pathToInstance
    TSP(const string &pathToInstance);

    /// @brief Creates a TSP copying the information from other instance
    /// @param other
    TSP(TSP const *other);

    /// @brief Get the upper bound of the search space. In this case the upper
    /// bound is the number of variables minus one since the starting city is
    /// duplicated at the end of the travel.
    /// @param i
    /// @return
    float getUpperLimit(int i) const override;

    /// @brief Get the optimization direction. Only for dimension = 0 and
    /// returns Minimize.
    /// @param i
    /// @return
    int getOptimizationDirection(const int i) const override;

    /// @brief Get the lower bound of the search space (1).
    /// @param i
    /// @return
    float getLowerLimit(int i) const override;

    /// @brief Get the name of the problem
    /// @return A string with the TSP acronym
    string getName() const override { return "TSP"; };

    /// @brief Get the matrix of distances between each city in the instance
    /// @return Vector of vectors of float with all distances
    const vector<vector<float>> &getDistances() const { return distances; }

    /// @brief Get the points of the instances as coordinates.
    /// @return Vector of coordinates
    const vector<coords> &getPoints() const { return points; }

    /// @brief Get the path to the instance if used.
    /// @return String with the location of the filesystem of the instance
    const std::string getPathToInstance() const { return pathToInstance; }

    /// @brief Evaluates a solution to the TSP
    /// @param solution
    void evaluate(IntFloatSolution &solution) const override;

    /// @brief Evaluates whether a solution is feasible of not regarding to the
    /// TSP formulation
    /// @param solution
    /// @return True if the solution is feasible, false otherwise.
    bool evaluateConstraints(IntFloatSolution &solution) const override;

    /// @brief Creates and return a random solution for the TSP
    /// @return
    IntFloatSolution createSolution() const override;

    /// \deprecated @brief Creates and return a random solution for the TSP
    /// @return
    IntFloatSolution createSolution(ParallelPRNG &engine) const override;

    /// @brief Reads the instance information from the filesystem
    /// @param path
    void readData(const string &path) override;

    /// @brief Creates and return the JSON representation of the TSP problem
    /// @return
    json to_json() const override;

   protected:
    void computeDistances();

   protected:
    std::string pathToInstance;      /*!< Path to the instance file */
    vector<coords> points;           /*!< Coordinates of cities (points) */
    vector<vector<float>> distances; /*!< Distance matrix  */
};

#endif