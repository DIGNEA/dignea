
#ifndef __DIVERSITY_EVOLUTION_H__
#define __DIVERSITY_EVOLUTION_H__

#include <dignea/core/Diversity.h>
#include <dignea/evolution/Evolution.h>

using namespace std;

/**
 * @brief Class which represents the diversity evolution of the execution of an
 * algorithm
 *
 * @tparam T
 * @tparam S
 */
template <typename T, class S>
class DiversityEvolution : public Evolution<S> {
   public:
    DiversityEvolution(unique_ptr<Diversity<T>>);

    virtual ~DiversityEvolution() = default;

    void update(const int &, const vector<S> &) override;

    Diversity<T> *getDivMetric() { return diversity.get(); }

   private:
    unique_ptr<Diversity<T>> diversity;
};

/**
 * @brief Construct a new Diversity Evolution< T,  S>:: Diversity
 * Evolution object
 *
 * @tparam T
 * @tparam S
 * @param diversity
 */
template <typename T, class S>
DiversityEvolution<T, S>::DiversityEvolution(unique_ptr<Diversity<T>> diversity)
    : Evolution<S>() {
    this->diversity = move(diversity);
}

/**
 * @brief Computes then diversity of the population at the checkpoint
 * and stores the results
 *
 * @tparam T
 * @tparam S
 * @param checkpoint
 * @param solutions
 */
template <typename T, class S>
void DiversityEvolution<T, S>::update(const int &checkpoint,
                                      const vector<S> &solutions) {
    vector<S> feasibles;
    for (const S &sol : solutions) {
        if (sol.getNumberOfCons() == 0 || sol.getConstraintCoeff() == 0) {
            feasibles.push_back(sol);
        }
    }
    this->checkpoints.push_back(checkpoint);
    this->solutions.push_back(this->diversity->compute(feasibles));
}

#endif  // __DIVERSITY_EVOLUTION_H__
