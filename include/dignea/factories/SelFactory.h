/**
 * @file SelFactory.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2021-06-22
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_SELFACTORY_H
#define DIGNEA_SELFACTORY_H

#include <dignea/core/Selection.h>
#include <dignea/core/Solution.h>
#include <dignea/selections/BinaryTournamentSelection.h>
#include <dignea/types/SelectionTypes.h>

/**
 * @brief Selection factory which allows the user to create Selection Operators
 * easily
 * @tparam S
 */
template <class S>
class SelFactory {
   private:
    map<SelType, function<unique_ptr<Selection<S>>()>> factories;

   public:
    SelFactory() {
        factories[SelType::Binary] = [] {
            return make_unique<BinaryTournamentSelection<S>>();
        };
    }

    /**
     * @brief Creates a unique pointer to a Selection Operator of type
     * @param type
     * @return
     */
    unique_ptr<Selection<S>> create(SelType type) { return factories[type](); }
};

#endif  // DIGNEA_SELFACTORY_H
