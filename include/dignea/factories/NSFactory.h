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
template <class S>
class NSFactory {
   private:
    map<NSType, function<unique_ptr<NoveltySearch<S>>(
                    unique_ptr<Distance<float>> dist, const float& thres,
                    const float& finalThresh, const int k, bool warmUp)>>
        factories;

   public:
    NSFactory() {
        factories[NSType::Standard] =
            [](unique_ptr<Distance<float>> dist, const float& thres,
               const float& finalThresh, const int k, bool warmUp) {
                return make_unique<NoveltySearch<S>>(move(dist), thres,
                                                     finalThresh, k);
            };
        factories[NSType::Features] =
            [](unique_ptr<Distance<float>> dist, const float& thres,
               const float& finalThresh, const int k, bool warmUp) {
                return make_unique<NSFeatures<S>>(move(dist), thres,
                                                  finalThresh, k, warmUp);
            };
        factories[NSType::Performance] =
            [](unique_ptr<Distance<float>> dist, const float& thres,
               const float& finalThresh, const int k, bool warmUp) {
                return make_unique<NSPerformance<S>>(move(dist), thres,
                                                     finalThresh, k);
            };
    }

    /**
     * @brief Creates a unique pointer to a Novelty Search algorithm of given
     * type. Variants are available at NSType.
     * @param type
     * @return
     */
    unique_ptr<NoveltySearch<S>> create(NSType type,
                                        unique_ptr<Distance<float>> dist,
                                        const float& thres,
                                        const float& finalThresh, const int k,
                                        bool warmUp) {
        return factories[type](move(dist), thres, finalThresh, k, warmUp);
    }
};

#endif
