/**
 * @file TSPDomain.h
 * @author Alejandro Marrero - amarrerd@ull.edu.es
 * @brief Evolutionary Traveling Salesman Problem.
 * Problem for the EIG to generate TSP Problem Instances
 * The number of variables (dimension) is set the number of cities
 * @version 0.1
 * @date 2022-03-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DIGNEA_ITSPPROBLEM_H
#define DIGNEA_ITSPPROBLEM_H

#include <dignea/generator/AbstractDomain.h>
#include <dignea/generator/instances/TSPInstance.h>
#include <dignea/problems/TSP.h>
#include <dignea/types/SolutionTypes.h>

/**
 * @brief Instance Generator TSP Problem.
 * Problem for the EIG to generate TSP Instances
 *
 */
class TSPDomain : public AbstractDomain<TSP, TSPInstance> {
   public:
    /// @brief Creates a default TSPDomain
    TSPDomain();

    /// @brief Creates a TSPDomain object with the number of variables.
    /// The number of variables (dimension) is set as the double of the
    /// number of variables which it is used to create an object.
    /// This is because the ITPSolution must be twice size of the nVars since it
    /// stores the pairs (x,y) coordinates for each city.
    /// The default limits for the coordinates are [0, 1000], eps = 5.0 and
    /// minNeighbors = 2.
    /// @param numberOfVars
    explicit TSPDomain(const int &numberOfVars, const int &numberOfObjs = 1);

    /// @brief Creates a TSPDomain object copying the information from other
    /// object
    /// @param prob
    TSPDomain(const TSPDomain *prob);

    virtual ~TSPDomain() = default;

    /// @brief Not allowed here
    /// @param pathToInstance
    TSPDomain(const string &pathToInstance) = delete;

    /// @brief Creates a TSPDomain with the DBSCAN parameters to compute the
    /// features.
    /// @param numberOfVars Number of variables
    /// @param maxL Upper bound
    /// @param minL Lower bound
    /// @param e Epsilon threshold to consider two points neighbors in DBSCAN
    /// @param minN Minimum number of neighbors to consider a cluster.
    TSPDomain(const int &numberOfVars, const int &numberOfObjs,
                const float &maxL, const float &minL, const float e = 5.0,
                const int minN = 2);

    /// @brief Generates a TSP problem with the information in the TSPInstance
    /// to be solved in the EIG by the algorithms in the portfolio.
    /// @param instance
    /// @return
    shared_ptr<TSP> genOptProblem(const TSPInstance &instance) const override;

    /// @brief Not implemented.
    /// @param solution
    void evaluate(TSPInstance &solution) const override { return; };

    /// @brief Creates a vector of initial instances (ITSPSolutions) for the EIG
    /// @param maxSolutions
    /// @return
    vector<TSPInstance> createSolutions(
        const int &maxSolutions) const override;

    /// @brief Performs problem dependent operations before evaluating the
    /// instances in the EIG
    /// @param
    void beforeEvaluation(vector<TSPInstance> &) override { return; };

    /// @brief Performs problem dependent operatiions after evaluating the
    /// instances in the EIG.
    /// Computes the features of the instance. For the TSP are:
    /// "size",   "mean_dist",     "std_dist",      "centroid", "radius",
    /// "cluster_ratio", "cluster_radius"
    /// @param
    void afterEvaluation(vector<TSPInstance> &) override;

    /// @brief Creates a single solution (TSP instance)
    /// @return
    TSPInstance createSolution() const override;

    /// \deprecated @brief Creates a single solution (TSP instance)
    /// @param engine
    /// @return
    TSPInstance createSolution(ParallelPRNG &engine) const override;

    /// @brief Not implemented here.
    /// @param solution
    /// @return
    bool evaluateConstraints(TSPInstance &solution) const override {
        return true;
    }
    /// @brief Returns the optimization direction for each dimension. Minimize
    /// in each case.
    /// @param i
    /// @return
    int getOptimizationDirection(const int i) const override;

    /// @brief Get the problem name
    /// @return a String with the information
    string getName() const override { return "TSPDomain"; }

    /// @brief Get the upper limit of the ith dimension (default = 1000)
    /// @param i
    /// @return
    float getUpperLimit(const int i) const override;

    /// @brief Get the lower limit of the ith dimension (default = 0)
    /// @param i
    /// @return
    float getLowerLimit(const int i) const override;

    /// @brief Set the upper limit for the ith dimension
    /// @param l
    void setUpperLimit(const float &l) { this->upperLimit = l; }

    /// @brief Set the lower limit for the ith dimension
    /// @param l
    void setLowerLimit(const float &l) { this->lowerLimit = l; }

    /// @brief Set the epsilon parameter for the DBSCAN algorithm
    /// @param e
    void setEPS(const float e) { eps = e; }

    /// @brief Get the epsilon parameter of the DBSCAN algorithm
    /// @return
    float getEPS() const { return eps; }

    /// @brief Set the minimum number of neighbors to consider a cluster in the
    /// DBSCAN algorithm
    /// @param m
    void setMinNeighbors(const int m) { minNeighbors = m; }

    /// @brief Get the minimum number of neighbors to consider a cluster in the
    /// DBSCAN algorithm
    /// @return
    int getMinNeighbors() const { return minNeighbors; }

    /// @brief Not implemented here.
    /// @param path
    void readData(const string &path) override{};

   private:
    coords computeCentroid(const vector<coords> &points);

    float instanceRadius(const vector<coords> &points, const coords &centroid);

    float distinctDistances(const vector<vector<float>> &distances);

   private:
    float upperLimit;
    float lowerLimit;
    float eps;
    int minNeighbors;
};

#endif