
#include <dignea/core/Solution.h>
#include <dignea/distances/Euclidean.h>
#include <dignea/distances/Hamming.h>
#include <dignea/generator/instances/KPInstance.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/KNN.h>

#include <algorithm>
#include <catch2/catch_all.hpp>
#include <random>
#include <vector>

TEST_CASE("KNN tests", "[KNN]") {
    SECTION("KNN with predefined int solutions") {
        vector<vector<float>> descriptors = {{1, 2, 3, 4, 5, 6, 7, 8, 9},
                                             {2, 3, 4, 5, 6, 7, 8, 9, 10},
                                             {3, 4, 5, 6, 7, 8, 9, 10, 11}};
        auto dist_1_2 = 3;
        auto dist_1_3 = 6;
        auto dist_2_3 = 3;
        const int nSolutions = 3;
        auto euclidean = make_unique<Euclidean<float>>();
        auto knn = make_unique<KNN>();
        neighMatrix nearest = knn->run(descriptors, euclidean.get(), 2);
        REQUIRE(nearest.size() == nSolutions);
        auto nearest_of_1 = nearest[0];
        REQUIRE(nearest_of_1.size() == 2);
        REQUIRE(nearest_of_1[0].first == dist_1_2);
        REQUIRE(nearest_of_1[0].second == 1);
        REQUIRE(nearest_of_1[1].first == dist_1_3);
        REQUIRE(nearest_of_1[1].second == 2);

        auto nearest_of_2 = nearest[1];
        REQUIRE(nearest_of_2.size() == 2);
        REQUIRE(nearest_of_2[0].first == dist_1_2);
        REQUIRE(nearest_of_2[0].second == 0);
        REQUIRE(nearest_of_2[1].first == dist_2_3);
        REQUIRE(nearest_of_2[1].second == 2);

        auto nearest_of_3 = nearest[2];
        REQUIRE(nearest_of_3.size() == 2);
        REQUIRE(nearest_of_3[0].first == dist_2_3);
        REQUIRE(nearest_of_3[0].second == 1);
        REQUIRE(nearest_of_3[1].first == dist_1_3);
        REQUIRE(nearest_of_3[1].second == 0);

        // KNN throws runtime error
        REQUIRE_THROWS(KNN().run(descriptors, euclidean.get(), 10));
    }

    SECTION("Test the sparseness function equals 0") {
        const int nSolutions = 3;
        const int k = 2;
        vector<vector<float>> descriptors = {{1, 2, 3, 4, 5, 6, 7, 8, 9},
                                             {1, 2, 3, 4, 5, 6, 7, 8, 9},
                                             {1, 2, 3, 4, 5, 6, 7, 8, 9}};

        auto euclidean = make_unique<Euclidean<float>>();
        vector spars = sparseness(descriptors, euclidean.get(), 3);
        REQUIRE(spars.size() == nSolutions);
        for (const float spar : spars) {
            REQUIRE(abs(spar - 0) < 1e-5);
        }
        // sparseness throws runtime error
        REQUIRE_THROWS(sparseness(descriptors, euclidean.get(), k * 10));
    }

    SECTION("Test the sparseness function with random pop") {
        const int nSolutions = 3;
        const int dimension = 9;
        const int k = 2;
        random_device randomDevice;
        mt19937 randomEngine(randomDevice());
        uniform_real_distribution<float> dist(1, 1000);
        vector<vector<float>> solutions;

        for (int i = 0; i < nSolutions; i++) {
            vector<float> features(dimension);
            std::generate(features.begin(), features.end(),
                          [&] { return dist(randomEngine); });

            solutions.push_back(features);
        }

        auto hamming = make_unique<Hamming<float>>();
        vector spars = sparseness(solutions, hamming.get(), 3);
        REQUIRE(spars.size() == nSolutions);
        for (const int spar : spars) {
            REQUIRE(spar != 0);
        }
        // sparseness throws runtime error
        REQUIRE_THROWS(sparseness(solutions, hamming.get(), k * 10));
    }
}
