//
// Created by amarrero on 21/6/21.
//

#ifndef DIGNEA_WEIGHTED_H
#define DIGNEA_WEIGHTED_H
#include <dignea/generator/AbstractInstance.h>
#include <dignea/generator/evaluations/InstanceFitness.h>
#include <dignea/searches/NoveltySearch.h>
#include <fmt/core.h>

#include <string>
#include <string_view>
#include <tuple>

/**
 * @brief Weighted Fitness Function used from evaluating the instances in EIG.
 * This functions performs a linear weighted sum of the performance fitness of
 * the instances and their novelty to calculate their final fitness
 *
 * @tparam AbstractInstance
 */
template <class AbstractInstance>
class Weighted {
   public:
    Weighted(const float &fRatio, const float &dRatio);

    virtual ~Weighted() = default;

    void computeFitness(vector<AbstractInstance> &solutions);

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
 * @tparam AbstractInstance
 * @param fRatio Weight of the fitness in the final fitness.
 * @param dRatio Weight of the novelty in the final fitness.
 */
template <class AbstractInstance>
Weighted<AbstractInstance>::Weighted(const float &fRatio, const float &dRatio)
    : fitnessRatio(fRatio), diversityRatio(dRatio), fitnessFunc() {}

/**
 * @brief Computes the actual fitness of the solutions based on the
 * fitness pre-defined in the instances and their diversity
 *
 * The selection of different ratios determine whether the generated instances
 * will be completely biased (and well clustered) or more dispersed
 * @tparam AbstractInstance
 * @param solutions
 */
template <class AbstractInstance>
void Weighted<AbstractInstance>::computeFitness(
    vector<AbstractInstance> &solutions) {
    for (unsigned int i = 0; i < solutions.size(); i++) {
        auto fullFitness = solutions[i].getBiasedFitness() * fitnessRatio +
                           solutions[i].getDiversity() * diversityRatio;

#ifdef DEBUG
        std::cout << fmt::format("{:>20}\t{:>20}\t{:>20}",
                                 solutions[i].getBiasedFitness(),
                                 solutions[i].getDiversity(), fullFitness)
                  << std::endl;
#endif

        solutions[i].setFitness(fullFitness);
    }
}

#endif  // DIGNEA_WEIGHTED_H
