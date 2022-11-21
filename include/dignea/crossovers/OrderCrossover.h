/**
 * @file OrderCrossover.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-10-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_ORDERCROSSOVER_H
#define DIGNEA_ORDERCROSSOVER_H

#include <dignea/core/Crossover.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/random/PseudoRandom.h>

#include <algorithm>
#include <vector>

using namespace std;

/**
 * @brief Class which defines the Order Crossover Operator from Eiben's book.
 * Works for permutation representations
 *
 * @tparam S
 */
template <class S = IntFloatSolution>
class OrderCrossover : public Crossover<S> {
   public:
    OrderCrossover() = default;

    virtual ~OrderCrossover() = default;

    inline void run(S &firstInd, S &secondInd) override;

    std::string getName() const override { return "Order Crossover"; };
};

/**
 * @brief Performs the Order Crossover Operator
 * This operator is used in permutation based problems like TSP. It fixes the
 * first and the last genes to zero (TSP constraint).
 *
 * @tparam S
 * @param firstInd
 * @param secondInd
 */
template <class S>
void OrderCrossover<S>::run(S &firstInd, S &secondInd) {
    vector secondIndVars = secondInd.getVariables();
    vector firstIndVars = firstInd.getVariables();
    const int size = secondIndVars.size() - 1;

    int fstPoint = 1;
    int sndPoint = 1;
    while (fstPoint == sndPoint) {
        fstPoint = PseudoRandom::randInt(1, size);
        sndPoint = PseudoRandom::randInt(1, size);
    }
    int start = min(fstPoint, sndPoint);
    int end = max(fstPoint, sndPoint);
    auto startCity = 0;

    vector fstOff{startCity};
    vector sndOff{startCity};
    // fstOff.push_back(startCity);
    // sndOff.push_back(startCity);
    for (int i = start; i < end; i++) {
        fstOff.push_back(firstIndVars[i]);
        sndOff.push_back(secondIndVars[i]);
    }

    for (int i = 0; i < size; i++) {
        auto genIdx = (end + i) % size;
        auto genFstInd = firstIndVars[genIdx];
        auto genSndInd = secondIndVars[genIdx];
        if (find(fstOff.begin(), fstOff.end(), genSndInd) == fstOff.end()) {
            fstOff.push_back(genSndInd);
        }
        if (find(sndOff.begin(), sndOff.end(), genFstInd) == sndOff.end()) {
            sndOff.push_back(genFstInd);
        }
    }
    fstOff.push_back(startCity);
    sndOff.push_back(startCity);
    std::rotate(fstOff.begin() + 1, fstOff.begin() + start, fstOff.end() - 1);
    std::rotate(sndOff.begin() + 1, sndOff.begin() + start, sndOff.end() - 1);
    for (int i = 1; i < size; i++) {
        firstIndVars[i] = fstOff[i];
        secondIndVars[i] = sndOff[i];
    }
    firstInd.setVariables(firstIndVars);
    secondInd.setVariables(secondIndVars);
}

#endif  // DIGNEA_ORDERCROSSOVER_H