//
// Created by amarrero on 8/3/21.
//

#include <dignea/mea/solutions/IKPSolution.h>

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

using namespace std;

IKPSolution::IKPSolution() : MEASolution<int, float>(), capacity(0) {}

IKPSolution::IKPSolution(const int &nVars)
    : MEASolution<int, float>(nVars, 1, 0), capacity(0) {}

IKPSolution::IKPSolution(const IKPSolution &solution)
    : MEASolution<int, float>(solution) {
    this->capacity = solution.capacity;
}

IKPSolution::IKPSolution(const IKPSolution *solution)
    : MEASolution<int, float>(solution) {
    this->capacity = solution->capacity;
}

/**
 * Sets the variables of the solution.
 * Expected to be twice of number of vars since it stores the pairs (wi,pi).
 * @param vars
 */
void IKPSolution::setVariables(const vector<int> &vars) {
    if (vars.size() != (unsigned int)this->nVars) {
        string where = "In IKPSolution::setVariables Vars.size() = " +
                       to_string(vars.size()) +
                       " != nVars = " + to_string(this->nVars);
        throw OutOfRange(where);
    }
    this->variables = vars;
}

/**
 * Creates a JSON object with the information of the IKPSolution
 * @return
 */
json IKPSolution::to_json() const {
    json data;
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
    vector keys = {"Q",     "max_p",   "max_w", "min_p",
                   "min_w", "avg_eff", "mean",  "std"};
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
 * @brief Generates a instance representation of the KP problem
 *
 * @param os
 */
void IKPSolution::to_instance(std::ostream &os) const {
    auto nItems = this->getNumberOfVars();
    os << nItems / 2 << std::endl
       << this->getCapacity() << std::endl
       << std::endl;
    vector vars = this->getVariables();
    for (int i = 0; i < nItems; i += 2) {
        os << vars[i] << " " << vars[i + 1] << std::endl;
    }
}
