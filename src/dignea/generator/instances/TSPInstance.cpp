/**
 * @file TSPInstance.cpp
 * @author Alejandro Marrero - amarred@ull.edu.es
 * @brief
 * @version 0.1
 * @date 2022-03-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

#include "dignea/generator/instances/TSPInstance.h"

using namespace std;

/**
 * @brief Construct a new TSPInstance::TSPInstance object
 *
 */
TSPInstance::TSPInstance() : AbstractInstance<float, float>() {}

/**
 * @brief Construct a new TSPInstance::TSPInstance object
 *
 * @param nVars
 */
TSPInstance::TSPInstance(const int &nVars, const int &nObjs)
    : AbstractInstance<float, float>(nVars, nObjs, 0) {}

/**
 * @brief Construct a new TSPInstance::TSPInstance object
 *
 * @param solution
 */
TSPInstance::TSPInstance(const TSPInstance &solution)
    : AbstractInstance<float, float>(solution) {}

/**
 * @brief Construct a new TSPInstance::TSPInstance object
 *
 * @param solution
 */
TSPInstance::TSPInstance(const TSPInstance *solution)
    : AbstractInstance<float, float>(solution) {}

/**
 * Sets the variables of the solution.
 * Expected to be  pairs (x,y).
 * @param vars
 */
void TSPInstance::setVariables(const vector<float> &vars) {
    if (vars.size() != (unsigned int)this->nVars) {
        string where = "In TSPInstance::setVariables Vars.size() = " +
                       to_string(vars.size()) +
                       " != nVars = " + to_string(this->nVars);
        throw OutOfRange(where);
    }
    this->variables = vars;
}

/**
 * @brief Converts the variables into a vector of pairs (x, y) coords
 *
 * @return coords
 */
vector<coords> TSPInstance::to_coords() const {
    vector<coords> coordinates;
    coordinates.reserve(this->variables.size());
    for (unsigned int i = 0; i < this->variables.size() - 1; i += 2) {
        coordinates.push_back({variables[i], variables[i + 1]});
    }
    return coordinates;
}

/**
 * Creates a JSON object with the information of the TSPInstance
 * @return
 */
json TSPInstance::to_json() const {
    json data;
    data["n_vars"] = this->nVars;
    data["n_objs"] = this->nObjs;
    data["coords"] = this->variables;
    data["diversity"] = this->diversity;
    data["biasedFitness"] = this->biasedFitness;
    data["fitness"] = this->fitness;
    data["conf_fitness"] = this->portFitness;
    vector keys = {
        "size",   "mean_dist",     "std_dist",       "c_x",          "c_y",
        "radius", "cluster_ratio", "cluster_radius", "fraction_dist"};
    vector values = this->getFeatures();
    if (values.empty()) {
        values = std::vector<float>(keys.size(), -1.0);
    }

    map<string, float> featuresMap;
    auto zip = [](const std::string &s, float f) {
        return std::make_pair(s, f);
    };
    transform(keys.begin(), keys.end(), values.begin(),
              inserter(featuresMap, featuresMap.end()), zip);
    data["features"] = featuresMap;
    return data;
}

/**
 * @brief Generates a instance representation of the TSP problem
 *
 * @param os
 */
void TSPInstance::to_instance(std::ostream &os) const {
    os << this->getNumberOfVars() / 2 << std::endl;
    vector vars = this->getVariables();
    for (unsigned int i = 0; i < vars.size() - 1; i += 2) {
        os << vars[i] << " " << vars[i + 1] << std::endl;
    }
}
