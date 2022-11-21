
#ifndef DIGNEA_AVG_AvgEvolution_H
#define DIGNEA_AVG_AvgEvolution_H

#include <dignea/evolution/Evolution.h>

using namespace std;

/**
 * @brief Class which represents the Average Fitness Evolution of the population
 * during the execution of an algorithm
 *
 * @tparam S
 */
template <class S>
class AvgEvolution : public Evolution<S> {
   public:
    AvgEvolution() = default;

    virtual ~AvgEvolution() = default;

    void update(const int &, const vector<S> &) override;
};

/**
 * @brief Includes solutions in the evolution data at the given checkpoint
 *
 * @tparam S
 * @param checkpoint
 * @param checkpointSs
 */
template <class S>
void AvgEvolution<S>::update(const int &checkpoint,
                             const vector<S> &checkpointSs) {
    float avgFitness = 0.0;
    for (const S &solution : checkpointSs) {
        if (solution.getNumberOfCons() == 0 ||
            solution.getConstraintCoeff() == 0) {
            avgFitness += solution.getFitness();
        }
    }
    avgFitness /= checkpointSs.size();
    this->checkpoints.push_back(checkpoint);
    this->solutions.push_back(avgFitness);
}

#endif  // DIGNEA_AVG_AvgEvolution_H
