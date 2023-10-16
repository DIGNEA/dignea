//
// Created by amarrero on 8/3/21.
//

#include <dignea/generator/instances/KPInstance.h>

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

using namespace std;

KPInstance::KPInstance()
    : AbstractInstance<int, float>(), capacity(0), reducedSpace(false) {}

KPInstance::KPInstance(const int &nVars, const int &nObjs, bool reducedSpace)
    : AbstractInstance<int, float>(nVars, nObjs, 0),
      capacity(0),
      reducedSpace(reducedSpace) {}

KPInstance::KPInstance(const KPInstance &solution)
    : AbstractInstance<int, float>(solution),
      capacity(solution.capacity),
      reducedSpace(solution.reducedSpace) {}

KPInstance::KPInstance(const KPInstance *solution)
    : AbstractInstance<int, float>(solution),
      capacity(solution->capacity),
      reducedSpace(solution->reducedSpace) {}

/**
 * @brief Copies a solution using the assignment operator
 *
 * @tparam V Variable type for the solution representation
 * @tparam O Objective type for the fitness
 * @param copy KPInstance to create a copy of
 * @return KPInstance
 */
KPInstance &KPInstance::operator=(const KPInstance &copy) {
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
    this->features = copy.features;
    this->diversity = copy.diversity;
    this->biasedFitness = copy.biasedFitness;
    this->portFitness = copy.portFitness;
    this->avgPortFitness = copy.avgPortFitness;
    this->reducedSpace = copy.isReducedSpace();
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
bool KPInstance::operator==(const KPInstance &other) const {
    bool equal = AbstractInstance<int, float>::operator==(other);
    if (equal) {
        return this->isReducedSpace() == other.isReducedSpace();
    } else
        return equal;
}

/**
 * @brief Sets the variables of the solution.
 * Expected to be twice of number of vars since it stores the pairs (wi,pi).
 * @param vars
 */
void KPInstance::setVariables(const vector<int> &vars) {
    if (vars.size() != (unsigned int)this->nVars) {
        string where = "In KPInstance::setVariables Vars.size() = " +
                       to_string(vars.size()) +
                       " != nVars = " + to_string(this->nVars);
        throw OutOfRange(where);
    }
    this->variables = vars;
}

/**
 * @brief Get the features. If reducedSpace is true in IKProblem the features
 * are the (x0, x1) principal components after applying PCA
 *
 * @return vector<float>
 */
vector<float> KPInstance::getFeatures() const {
    // Consider features not computed yet
    if (features.empty()) {
        return {};
    } else {
        if (reducedSpace) {
            return {features[0], features[1]};
        } else {
            return features;
        }
    }
};

/**
 * @brief Creates a JSON object with the information of the KPInstance
 * @return
 */
json KPInstance::to_json() const {
    json data = AbstractInstance<int, float>::to_json();
    int nItems = this->nVars;
    vector<int> weights;
    vector<int> profits;
    weights.reserve(nItems / 2);
    profits.reserve(nItems / 2);
    for (int i = 0; i < nItems; i += 2) {
        weights.push_back(this->variables[i]);
        profits.push_back(this->variables[i + 1]);
    }
    data["capacity"] = this->capacity;
    data["weights"] = weights;
    data["profits"] = profits;
    data["n_vars"] = this->nVars;
    data["diversity"] = this->diversity;
    data["biasedFitness"] = this->biasedFitness;
    data["fitness"] = this->fitness;
    data["conf_fitness"] = this->portFitness;
    data["isReducedSpace"] = this->reducedSpace;
    vector<string> keys;  // Using vector to keep the order
    if (reducedSpace) {
        keys = {"x0",    "x1",   "Q",     "avg_eff", "max_p",
                "max_w", "mean", "min_p", "min_w",   "std"};
    } else {
        keys = {"Q",     "max_p",   "max_w", "min_p",
                "min_w", "avg_eff", "mean",  "std"};
    }

    // vector values = this->getFeatures();
    // Instead of using the getFeatures() we access directly to the values
    // to avoid getFeatures() return (x0, x1)
    vector values = this->features;

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
 * @brief Generates a instance representation of the KP problem
 *
 * @param os
 */
void KPInstance::to_instance(std::ostream &os) const {
    auto nItems = this->getNumberOfVars();
    os << nItems / 2 << std::endl
       << this->getCapacity() << std::endl
       << std::endl;
    vector vars = this->getVariables();
    for (int i = 0; i < nItems; i += 2) {
        os << vars[i] << " " << vars[i + 1] << std::endl;
    }
}
