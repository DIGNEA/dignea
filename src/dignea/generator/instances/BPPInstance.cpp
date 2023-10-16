/**
 * @file BPPInstance.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-05-31
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <dignea/generator/instances/BPPInstance.h>

#include <algorithm>
#include <map>
#include <ranges>
#include <utility>
#include <vector>

using namespace std;

BPPInstance::BPPInstance() : AbstractInstance<int, int>(), capacity(0) {}

BPPInstance::BPPInstance(const int &nVars, const int &nObjs)
    : AbstractInstance<int, int>(nVars, nObjs, 0), capacity(0) {}

BPPInstance::BPPInstance(const BPPInstance &solution)
    : AbstractInstance<int, int>(solution), capacity(0) {
    this->capacity = solution.capacity;
}

BPPInstance::BPPInstance(const BPPInstance *solution)
    : AbstractInstance<int, int>(solution), capacity(0) {
    this->capacity = solution->capacity;
}

/**
 * @brief Copies a solution using the assignment operator
 *
 * @tparam V Variable type for the solution representation
 * @tparam O Objective type for the fitness
 * @param copy BPPInstance to create a copy of
 * @return BPPInstance
 */
BPPInstance &BPPInstance::operator=(const BPPInstance &copy) {
    if (this == &copy) {
        return *this;
    }
    this->nVars = copy.nVars;
    this->nObjs = copy.nObjs;
    this->nCons = copy.nCons;
    this->fitness = copy.fitness;
    this->capacity = copy.capacity;
    this->constraintCoeff = copy.constraintCoeff;
    this->variables = copy.variables;
    this->objectives = copy.objectives;
    this->constraints = copy.constraints;
    this->features = copy.features;
    this->diversity = copy.diversity;
    this->biasedFitness = copy.biasedFitness;
    this->portFitness = copy.portFitness;
    this->avgPortFitness = copy.avgPortFitness;
    this->capacity = copy.capacity;
    return *this;
}

/**
 * @brief Compares whether two solutions are equal
 *
 * @tparam V Variable type for the solution representation
 * @tparam O Objective type for the fitness
 * @param other AbstractInstance to compare to
 * @return true
 * @return false
 */
bool BPPInstance::operator==(const BPPInstance &other) const {
    return this->capacity == other.capacity &&
           AbstractInstance<int, int>::operator==(other);
}

/**
 * @brief Sets the variables of the solution.
 * Expected to be twice of number of vars since it stores the pairs (wi,pi).
 * @param vars
 */
void BPPInstance::setVariables(const vector<int> &vars) {
    if (vars.size() != (unsigned int)this->nVars) {
        string where = "In BPPInstance::setVariables Vars.size() = " +
                       to_string(vars.size()) +
                       " != nVars = " + to_string(this->nVars);
        throw OutOfRange(where);
    }
    this->variables = vars;
}

/**
 * @brief Get the features.
 * @return vector<float>
 */
vector<float> BPPInstance::getFeatures() const {
    // Consider features not computed yet
    if (features.empty()) {
        return {};
    } else {
        return features;
    }
};

/**
 * @brief Creates a JSON object with the information of the BPPInstance
 * @return
 */
json BPPInstance::to_json() const {
    json data = AbstractInstance<int, int>::to_json();

    vector tempVars = this->variables;
    data["capacity"] = this->capacity;
    data["items"] = tempVars;
    data["n_vars"] = this->nVars;
    data["diversity"] = this->diversity;
    data["biasedFitness"] = this->biasedFitness;
    data["fitness"] = this->fitness;
    data["conf_fitness"] = this->portFitness;

    vector<string> keys;
    // Using vector to keep the order
    // Mean, Std, Median,
    // Max Weight, Min Weight, Tiny, Small, Medium, Large, Huge
    keys = {"mean", "std",   "median", "max",   "min",
            "tiny", "small", "medium", "large", "huge"};
    vector values = this->features;
    if (values.empty()) {
        values = std::vector<float>(keys.size(), -1.0);
    }

    // Creates a MAP with the keys and values of the features
    // Do not update this code
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
 * @brief Generates a BPPInstance file with .bpp extension from the instance
 * information
 *
 * @param os
 */
void BPPInstance::to_instance(std::ostream &os) const {
    auto nItems = this->getNumberOfVars();
    os << nItems << " " << this->capacity << std::endl << std::endl;
    for (auto &i : this->variables) {
        os << i << std::endl;
    }
}
