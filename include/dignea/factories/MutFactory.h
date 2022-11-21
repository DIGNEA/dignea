//
// Created by amarrero on 21/6/21.
//

#ifndef DIGNEA_MUTFACTORY_H
#define DIGNEA_MUTFACTORY_H

#include <dignea/core/Mutation.h>
#include <dignea/core/Problem.h>
#include <dignea/core/Solution.h>
#include <dignea/mutations/SwapMutation.h>
#include <dignea/mutations/UniformAllMutation.h>
#include <dignea/mutations/UniformOneMutation.h>
#include <dignea/types/MutationTypes.h>

/**
 * @brief Mutation factory which allows the user to create Mutation Operators
 * easily
 * @tparam Problem
 * @tparam S
 */
template <class S>
class MutFactory {
   private:
    map<MutType, function<unique_ptr<Mutation<S>>()>> factories;

   public:
    MutFactory() {
        factories[MutType::UniformAll] = [] {
            return make_unique<UniformAllMutation<S>>();
        };
        factories[MutType::UniformOne] = [] {
            return make_unique<UniformOneMutation<S>>();
        };
        factories[MutType::SwapMutation] = [] {
            return make_unique<SwapMutation<S>>();
        };
    }

    /**
     * @brief Creates a unique pointer to a Mutation Operator of type
     * @param type
     * @return
     */
    unique_ptr<Mutation<S>> create(MutType type) { return factories[type](); }
};

#endif  // DIGNEA_MUTFACTORY_H
