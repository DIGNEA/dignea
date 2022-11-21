/**
 * @file KPNR.h
 * @author Alejandro Marrero (amarred@ull.edu.es)
 * @brief
 * @version 0.1
 * @date  29/1/21.
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __DIGNEA_KPNR_H__
#define __DIGNEA_KPNR_H__

#include <dignea/utilities/random/ParallelPRNG.h>

#include "KP.h"

/**
 * @brief Knapsack Problem without repairing operator.
 *
 */

class KPNR : public KP {
   public:
    KPNR();

    virtual ~KPNR();

    explicit KPNR(const int &numberOfVars);

    KPNR(const string &pathToInstance);

    KPNR(const int &numberOfVars, const int &capacity,
         const vector<int> &weights, const vector<int> &profits);

    // Generacion de una instancia del KP de manera aleatoria con los datos
    // proporcionadoss
    KPNR(const int &numberOfVars, const int &lowerWeight,
         const int &upperWeight, const int &lowerProfit,
         const int &upperProfit);

    void evaluate(BoolFloatSolution &solution) const override;
};

#endif  // __DIGNEA_KPNR_H__
