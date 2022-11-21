/**
 * @file ITSPProblem.cpp
 * @author Alejandro Marrero (amarred@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-03-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "dignea/mea/problems/ITSPProblem.h"

#include <dignea/mea/solutions/ITSPSolution.h>
#include <dignea/utilities/Statistics.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <cmath>
#include <set>

#include "dbscan.h"

ITSPProblem::ITSPProblem()
    : MEAProblem<TSP, ITSPSolution>(1, 1, 0),
      upperLimit(1000),
      lowerLimit(0),
      eps(5.0),
      minNeighbors(2) {}

/// @brief Creates a ITSPProblem object with the number of variables.
/// The number of variables (dimension) is set as the double of the
/// number of variables which it is used to create an object.
/// This is because the ITPSolution must be twice size of the nVars since it
/// stores the pairs (x,y) coordinates for each city.
/// The default limits for the coordinates are [0, 1000], eps = 5.0 and
/// minNeighbors = 2.
/// @param numberOfVars
ITSPProblem::ITSPProblem(const int &numberOfVars)
    : MEAProblem<TSP, ITSPSolution>(numberOfVars, 1, 0),
      upperLimit(1000),
      lowerLimit(0),
      eps(5.0),
      minNeighbors(2) {}

/// @brief Creates a ITSPProblem with the DBSCAN parameters to compute the
/// features.
/// @param numberOfVars Number of variables
/// @param maxL Upper bound
/// @param minL Lower bound
/// @param e Epsilon threshold to consider two points neighbors in DBSCAN
/// @param minN Minimum number of neighbors to consider a cluster.
ITSPProblem::ITSPProblem(const int &numberOfVars, const float &maxL,
                         const float &minL, const float e, const int minN)
    : MEAProblem<TSP, ITSPSolution>(numberOfVars, 1, 0),
      upperLimit(maxL),
      lowerLimit(minL),
      eps(e),
      minNeighbors(minN) {}

ITSPProblem::ITSPProblem(const ITSPProblem *other)
    : MEAProblem<TSP, ITSPSolution>(other->numberOfVars, 1, 0) {
    upperLimit = other->upperLimit;
    lowerLimit = other->lowerLimit;
    eps = other->eps;
    minNeighbors = other->minNeighbors;
}

/// @brief Creates a vector of initial instances (ITSPSolutions) for the MEA
/// @param maxSolutions
/// @return
vector<ITSPSolution> ITSPProblem::createSolutions(
    const int &maxSolutions) const {
    vector<ITSPSolution> solutions;
    solutions.reserve(maxSolutions);
    for (int i = 0; i < maxSolutions; i++) {
        vector<float> vars;
        vars.reserve(this->numberOfVars);
        for (int j = 0; j < this->numberOfVars; j++) {
            float x =
                PseudoRandom::randDouble(this->lowerLimit, this->upperLimit);
            vars.push_back(x);
        }
        ITSPSolution solution(this->numberOfVars);
        solution.setVariables(vars);
        solutions.push_back(solution);
    }
    return solutions;
}

ITSPSolution ITSPProblem::createSolution() const {
    vector<float> vars;
    vars.reserve(this->numberOfVars);
    for (int i = 0; i < this->numberOfVars; i++) {
        float x = PseudoRandom::randDouble(this->lowerLimit, this->upperLimit);
        vars.push_back(x);
    }
    ITSPSolution solution(this->numberOfVars);
    solution.setVariables(vars);
    return solution;
}

ITSPSolution ITSPProblem::createSolution(ParallelPRNG &engine) const {
    vector<float> vars;
    vars.reserve(this->numberOfVars);
    for (int i = 0; i < this->numberOfVars; i++) {
        float x = PseudoRandom::randDouble(this->lowerLimit, this->upperLimit);
        vars.push_back(x);
    }
    ITSPSolution solution(this->numberOfVars);
    solution.setVariables(vars);
    return solution;
}

float ITSPProblem::getUpperLimit(const int i) const {
    if (i < 0 || i >= this->numberOfVars) {
        std::string where =
            "getUpperLimit in ITSP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return upperLimit;
}

float ITSPProblem::getLowerLimit(const int i) const {
    if (i < 0 || i >= this->numberOfVars) {
        std::string where =
            "getLowerLimit in EKPP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return lowerLimit;
}

/// @brief Generates a TSP problem with the information in the ITSPSolution
/// to be solved in the MEA by the algorithms in the portfolio.
/// @param instance
/// @return
shared_ptr<TSP> ITSPProblem::genOptProblem(const ITSPSolution &instance) const {
    vector<coords> variables = instance.to_coords();
    if (instance.getVariables().size() != (unsigned int)this->numberOfVars) {
        string where = "instance.variables.size() != Problem inner dimension";
        throw OutOfRange(where);
    }
    return make_shared<TSP>(variables.size(), variables);
}

/// @brief Returns the optimization direction for each dimension. Minimize
/// in each case.
/// @param i
/// @return
int ITSPProblem::getOptimizationDirection(const int i) const {
    if (i < 0 || i >= this->getNumberOfObjs()) {
        std::string where =
            "getOptimizationDirection in ITSProblem with index = " +
            to_string(i);
        throw(OutOfRange(where));
    }
    return Minimize;
}

/**
 * @brief Computes the centroid of the solution points given by parameter
 *  The centroid is a (x, y) point
 *
 * @param points
 * @return coords
 */
coords ITSPProblem::computeCentroid(const vector<coords> &points) {
    auto cX = 0.0f;
    auto cY = 0.0f;
    for (auto [x, y] : points) {
        cX += x;
        cY += y;
    }
    cX /= points.size();
    cY /= points.size();
    return {cX, cY};
}

/**
 * @brief Method to compute the radius of a TSP instance. The radius of a TSP
 * instance is defined as as the mean distance from each city to the centroid
 *
 * Definition extracted from:
 *
 * Smith-Miles, K., Van Hemert, J., & Lim, X. Y. (2010). Understanding TSP
 * difficulty by learning from evolved instances. Lecture Notes in Computer
 * Science (Including Subseries Lecture Notes in Artificial Intelligence and
 * Lecture Notes in Bioinformatics), 6073 LNCS, 266–280.
 * https://doi.org/10.1007/978-3-642-13800-3_29
 *
 * @param points
 * @param centroid
 * @return float
 */
float ITSPProblem::instanceRadius(const vector<coords> &points,
                                  const coords &centroid) {
    auto radius = 0.0f;
    for (auto [x, y] : points) {
        radius += sqrt(((x - centroid.first) * (x - centroid.first)) +
                       ((y - centroid.second) * (y - centroid.second)));
    }
    return radius / points.size();
}

/**
 * @brief Method to compute the fraction of distinct distances in the distance
 * matrix D.
 *
 * @param solution
 * @return float
 */
float ITSPProblem::distinctDistances(const vector<vector<float>> &distances) {
    std::set<float> distinctDist;
    int counter = 0;
    for (unsigned int i = 0; i < distances.size(); i++) {
        for (unsigned int j = i + 1; j < distances[i].size(); j++) {
            distinctDist.insert(distances[i][j]);
            counter++;
        }
    }
    return counter / distinctDist.size();
}

/// @brief Performs problem dependent operatiions after evaluating the
/// instances in the MEA.
/// Computes the features of the instance. For the TSP are:
/// "size",   "mean_dist",     "std_dist",      "centroid", "radius",
// "cluster_ratio", "cluster_radius"
/// @param
void ITSPProblem::afterEvaluation(vector<ITSPSolution> &solutions) {
    for (ITSPSolution &solution : solutions) {
        vector vars = solution.getVariables();
        auto distances = this->genOptProblem(solution)->getDistances();
        auto points = solution.to_coords();

        DBSCAN dbscan(minNeighbors, eps, points);
        auto nClusters = dbscan.run();
        float clusterRatio = -1.0f;
        float clusterRadius = -1.0f;
        if (nClusters != 0) {
            clusterRatio = vars.size() / nClusters;
            clusterRadius = dbscan.radiusOfClusters();
        }
        if (clusterRatio == std::numeric_limits<float>::infinity() ||
            isnan(clusterRatio)) {
            clusterRatio = -1.0f;
        }
        if (isnan(clusterRadius)) {
            clusterRadius = -1.0f;
        }
        coords centroid = computeCentroid(points);
        float radius = instanceRadius(points, centroid);
        float fractionDist = distinctDistances(distances);
        float meanDist = mean(distances);
        float stdDist = standardDev(meanDist, distances);

        vector<float> features = {
            (float)vars.size(), meanDist,        stdDist,
            centroid.first,     centroid.second, radius,
            clusterRatio,       clusterRadius,   fractionDist};

        solution.setFeatures(features);
    }
}
