/**
 * @file RepFactory.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __DIGNEA_REPFACTORY_H__
#define __DIGNEA_REPFACTORY_H__

#include <dignea/core/Replacement.h>
#include <dignea/core/Solution.h>
#include <dignea/replacements/EGenerational.h>
#include <dignea/replacements/FirstImprove.h>
#include <dignea/replacements/ReplaceWorst.h>
#include <dignea/types/ReplacementTypes.h>

/**
 * @brief Replacement Factory which allows the user to easily create Replacement
 * Operators
 *
 * @tparam S
 */
template <class S>
class RepFactory {
   private:
    map<ReplacementTypes, function<unique_ptr<Replacement<S>>()>> factories;

   public:
    RepFactory() {
        factories[ReplacementTypes::Generational] = [] {
            return make_unique<EGenerational<S>>();
        };
        factories[ReplacementTypes::FirstImprove] = [] {
            return make_unique<FirstImprove<S>>();
        };
        factories[ReplacementTypes::ReplaceWorst] = [] {
            return make_unique<ReplaceWorst<S>>();
        };
    }

    /**
     * @brief Creates a unique pointer to a Selection Operator of type
     * @param type
     * @return
     */
    unique_ptr<Replacement<S>> create(ReplacementTypes type) {
        return factories[type]();
    }
};

#endif  // __DIGNEA_REPFACTORY_H__
