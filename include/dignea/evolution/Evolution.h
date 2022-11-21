
#ifndef DIGNEA_EVOLUTION_H
#define DIGNEA_EVOLUTION_H

#include <dignea/core/Solution.h>
#include <dignea/utilities/Sorter.h>
#include <dignea/utilities/exceptions/Mismatch.h>

#include <iostream>
#include <map>
#include <vector>

using namespace std;

/**
 * @brief Class which represents the evolution of the Best Fitness found in the
 * population during the execution of an algorithm
 *
 * @tparam S
 */
template <class S>
class Evolution {
   public:
    Evolution() = default;

    virtual ~Evolution() = default;

    virtual void update(const int &, const vector<S> &);

    virtual map<int, float> results();

   protected:
    vector<float> solutions;
    vector<int> checkpoints;
};

/**
 * @brief Includes solutions in the evolution data at the given checkpoint
 *
 * @tparam S
 * @param checkpoint
 * @param checkpointSs
 */
template <class S>
void Evolution<S>::update(const int &checkpoint,
                          const vector<S> &checkpointSs) {
    vector<S> copy;
    copy.reserve(checkpointSs.size());
    for (const S &solution : checkpointSs) {
        if (solution.getNumberOfCons() == 0 ||
            solution.getConstraintCoeff() == 0) {
            copy.push_back(solution);
        }
    }
    // En caso de estar vacío no incluimos el checkpoint
    if (!copy.empty()) {
        sortByFitness(copy);
        checkpoints.push_back(checkpoint);
        solutions.push_back(copy[0].getFitness());
    }
}

/**
 * Creates a map with the results of each checkpoint
 * @tparam S
 * @return
 */
template <class S>
map<int, float> Evolution<S>::results() {
    if (checkpoints.size() != solutions.size()) {
        string where =
            "checkpoints.size() != solutions.size() in class Evolution";
        throw Mismatch(where);
    }
    map<int, float> evolutionResults;
    const int size = checkpoints.size();
    for (int i = 0; i < size; i++) {
        evolutionResults.insert({checkpoints[i], solutions[i]});
    }
    return evolutionResults;
}

#endif  // DIGNEA_EVOLUTION_H
