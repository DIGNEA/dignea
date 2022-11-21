/**
 * @file NSFactory.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-10-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_NSFACTORY_H
#define DIGNEA_NSFACTORY_H

#include <dignea/searches/NSFeatures.h>
#include <dignea/searches/NSPerformance.h>
#include <dignea/searches/NoveltySearch.h>
#include <dignea/types/NSTypes.h>

#include <string>

using namespace std;

/**
 * @brief Novelty Search factory which allows the user to create Novelty Search
 * algorithms on the go.
 * @tparam S
 */
template <class MS, typename T = float>
class NSFactory {
   private:
    map<NSType,
        function<unique_ptr<NoveltySearch<MS, T>>(
            unique_ptr<Distance<T>> dist, const float& thres, const int k)>>
        factories;

   public:
    NSFactory() {
        factories[NSType::Standard] = [](unique_ptr<Distance<T>> dist,
                                         const float& thres, const int k) {
            return make_unique<NoveltySearch<MS, T>>(move(dist), 1000, thres,
                                                     k);
        };
        factories[NSType::Features] = [](unique_ptr<Distance<T>> dist,
                                         const float& thres, const int k) {
            return make_unique<NSFeatures<MS>>(move(dist), 1000, thres, k);
        };
        factories[NSType::Performance] = [](unique_ptr<Distance<T>> dist,
                                            const float& thres, const int k) {
            return make_unique<NSPerformance<MS>>(move(dist), 1000, thres, k);
        };
    }

    /**
     * @brief Creates a unique pointer to a Novelty Search algorithm of given
     * type. Variants are available at NSType.
     * @param type
     * @return
     */
    unique_ptr<NoveltySearch<MS, T>> create(NSType type,
                                            unique_ptr<Distance<T>> dist,
                                            const float& thres, const int k) {
        return factories[type](move(dist), thres, k);
    }
};

#endif
