//
// Created by amarrero on 21/6/21.
//

#ifndef DIGNEA_CXFACTORY_H
#define DIGNEA_CXFACTORY_H

#include <dignea/core/Crossover.h>
#include <dignea/crossovers/OrderCrossover.h>
#include <dignea/crossovers/UniformCrossover.h>
#include <dignea/types/CrossoverTypes.h>

/**
 * @brief Crossover factory which allows the user to create Crossover operators
 * easily
 * @tparam S
 */
template <class S>
class CXFactory {
   private:
    map<CXType, function<unique_ptr<Crossover<S>>()>> factories;

   public:
    CXFactory() {
        factories[CXType::Uniform] = [] {
            return make_unique<UniformCrossover<S>>();
        };
        factories[CXType::Order] = [] {
            return make_unique<OrderCrossover<S>>();
        };
    }

    /**
     * @brief Creates a unique pointer to a Crossover Operator of the given
     * type. Variants are available at CXType.
     * @param type
     * @return
     */
    unique_ptr<Crossover<S>> create(CXType type) { return factories[type](); }
};

#endif  // DIGNEA_CXFACTORY_H
