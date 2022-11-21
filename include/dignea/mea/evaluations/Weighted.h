//
// Created by amarrero on 21/6/21.
//

#ifndef DIGNEA_WEIGHTED_H
#define DIGNEA_WEIGHTED_H

#include <dignea/mea/MEASolution.h>
#include <dignea/mea/evaluations/InstanceFitness.h>
#include <dignea/searches/NoveltySearch.h>

#include <tuple>

/**
 * @brief Weighted Fitness Function used from evaluating the instances in MEA.
 * This functions performs a linear weighted sum of the performance fitness of
 * the instances and their novelty to calculate their final fitness
 *
 * @tparam MEASolution
 */
template <class MEASolution>
class Weighted {
   public:
    Weighted(const float &fRatio, const float &dRatio);

    virtual ~Weighted() = default;

    void computeFitness(vector<MEASolution> &solutions);

    /**
     * @brief Returns a tuple containing the fitness and novelty ratio in that
     * order!
     *
     * @return std::tuple<float, float>
     */
    std::tuple<float, float> getFAndNRatios() {
        return {fitnessRatio, diversityRatio};
    }

   private:
    float fitnessRatio;
    float diversityRatio;
    unique_ptr<InstanceFitness> fitnessFunc;
};

/**
 * @brief Construct a new Weighted object with the corresponding fitness ratio
 * and diversity ratio.
 *
 * @tparam MEASolution
 * @param fRatio Weight of the fitness in the final fitness.
 * @param dRatio Weight of the novelty in the final fitness.
 */
template <class MEASolution>
Weighted<MEASolution>::Weighted(const float &fRatio, const float &dRatio)
    : fitnessRatio(fRatio), diversityRatio(dRatio) {}

/**
 * @brief Computes the actual fitness of the solutions based on the
 * fitness pre-defined in the instances and their diversity
 *
 * The selection of different ratios determine whether the generated instances
 * will be completely biased (and well clustered) or more dispersed
 * @tparam MEASolution
 * @param solutions
 */
template <class MEASolution>
void Weighted<MEASolution>::computeFitness(vector<MEASolution> &solutions) {
    for (unsigned int i = 0; i < solutions.size(); i++) {
        auto fullFitness = solutions[i].getBiasedFitness() * fitnessRatio +
                           solutions[i].getDiversity() * diversityRatio;

#ifdef DEBUG
        std::cout << "RF: " << solutions[i].getBiasedFitness()
                  << " D: " << solutions[i].getDiversity()
                  << " FF: " << fullFitness << std::endl;
#endif
        solutions[i].setFitness(fullFitness);
    }
}

#endif  // DIGNEA_WEIGHTED_H
