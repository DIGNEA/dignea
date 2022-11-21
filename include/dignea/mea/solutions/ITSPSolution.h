/**
 * @file ITSPSolution.h
 * @author Alejandro Marrero - amarrerd@ull.edu.es
 * @brief Evolutionary TSP Solution.
 * Class which represents the solution for the Instance Generation TSP Problem
 * (ITSPP). This class is used to represent an actual TSP Problem instance.
 *
 * @version 0.1
 * @date 2022-03-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DIGNEA_ITSPSOLUTION_H
#define DIGNEA_ITSPSOLUTION_H

#include <dignea/mea/MEASolution.h>

#include <utility>

using coords = std::pair<float, float>;

/**
 * @brief Instance Traveling Salesman Problem Solution.
 * Class which represents the solution for the Instance Traveling Salesman
 * Problem (ITSPProblem).
 *  The variables inherited from the Solution<float, float> class contains the
 * pairs of coordinates for each point (city) in the instance.
 *      [(x0,y0), ..., (xn-1,yn-1)] in a single array.
 *
 */
class ITSPSolution : public MEASolution<float, float> {
   public:
    /// @brief Creates a default ITSPSolution
    ITSPSolution();

    /// @brief Creates a ITSPSolution with a specified dimension. Notice that
    /// passing nVars = 100 will create a 50 cities since the number of
    /// variables must be divided by two to contain the coordinates for each
    /// city.
    /// @param nVars
    explicit ITSPSolution(const int &nVars);

    /// @brief Creates a new ITSPSolution object with the information from other
    /// solution
    /// @param solution
    ITSPSolution(const ITSPSolution &solution);

    /// @brief Creates a new ITSPSolution object with the information from other
    /// solution
    /// @param solution
    explicit ITSPSolution(const ITSPSolution *solution);

    /// @brief Cannot be used in the MEASolution
    /// @param ptr
    ITSPSolution(unique_ptr<ITSPSolution> &ptr) = delete;

    virtual ~ITSPSolution() = default;

    /// @brief Set the variables for the instance
    /// @param vars
    void setVariables(const vector<float> &vars) override;

    /// @brief Creates and returns a JSON object with the information from the
    /// TSP.
    /// @return
    json to_json() const override;

    /// @brief Generates the instance representation of the TSP and puts it in
    /// the ostream
    /// @param os
    virtual void to_instance(std::ostream &os) const;

    /// @brief Generates the vector of coordinates for the instance. Look the
    /// TSP getPoints() method.
    /// @return
    vector<coords> to_coords() const;
};

#endif