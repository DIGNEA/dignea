
#include <dignea/core/Solution.h>
#include <dignea/distances/Euclidean.h>
#include <dignea/distances/Hamming.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/KNN.h>

#include <algorithm>
#include <catch2/catch_all.hpp>
#include <random>
#include <vector>

TEST_CASE("KNN tests", "[KNN]") {
    SECTION("KNN with int solutions") {
        const int nSolutions = 3;
        const int dimension = 100;
        const int objs = 1;
        const int k = 3;
        vector<IntDoubleSolution> solutions;
        for (int i = 0; i < nSolutions; i++) {
            IntDoubleSolution solution(dimension, objs);
            vector<int> vars(dimension);
            std::iota(vars.begin(), vars.end(), 0);
            solution.setVariables(vars);
            solutions.push_back(solution);
        }
        auto hamming = make_unique<Hamming<int>>();
        IntDoubleSolution ind = solutions[0];
        neighVector<int> nearest = KNN().run(ind, solutions, hamming.get());
        REQUIRE(nearest.size() == k);
        for (int i = 0; i < k; i++) {
            REQUIRE(solutions[nearest[i].second] == ind);
            auto dist =
                hamming->compute(solutions[nearest[i].second].getVariables(),
                                 ind.getVariables());
            REQUIRE(dist == 0);
        }
        // KNN throws runtime error
        REQUIRE_THROWS(KNN().run(ind, solutions, hamming.get(), k * 10));
    }
    SECTION("KNN with predefined int solutions") {
        const int nSolutions = 3;
        const int dimension = 9;
        const int objs = 1;
        const int k = 2;
        vector<IntDoubleSolution> solutions;
        vector<vector<int>> vars = {{1, 2, 3, 4, 5, 6, 7, 8, 9},
                                    {2, 3, 4, 5, 6, 7, 8, 9, 10},
                                    {3, 4, 5, 6, 7, 8, 9, 10, 11}};
        auto dist_1_2 = 3;
        auto dist_1_3 = 6;
        auto dist_2_3 = 3;
        for (int i = 0; i < nSolutions; i++) {
            IntDoubleSolution solution(dimension, objs);
            solution.setVariables(vars[i]);
            solutions.push_back(solution);
        }
        auto euclidean = make_unique<Euclidean<int>>();
        auto knn = make_unique<KNN>();
        neighMatrix<int> nearest = knn->run(solutions, euclidean.get(), k);
        REQUIRE(nearest.size() == nSolutions);
        auto nearest_of_1 = nearest[0];
        REQUIRE(nearest_of_1.size() == k);
        REQUIRE(nearest_of_1[0].first == dist_1_2);
        REQUIRE(nearest_of_1[0].second == 1);
        REQUIRE(nearest_of_1[1].first == dist_1_3);
        REQUIRE(nearest_of_1[1].second == 2);

        auto nearest_of_2 = nearest[1];
        REQUIRE(nearest_of_2.size() == k);
        REQUIRE(nearest_of_2[0].first == dist_1_2);
        REQUIRE(nearest_of_2[0].second == 0);
        REQUIRE(nearest_of_2[1].first == dist_2_3);
        REQUIRE(nearest_of_2[1].second == 2);

        auto nearest_of_3 = nearest[2];
        REQUIRE(nearest_of_3.size() == k);
        REQUIRE(nearest_of_3[0].first == dist_2_3);
        REQUIRE(nearest_of_3[0].second == 1);
        REQUIRE(nearest_of_3[1].first == dist_1_3);
        REQUIRE(nearest_of_3[1].second == 0);

        // KNN throws runtime error
        REQUIRE_THROWS(KNN().run(solutions, euclidean.get(), k * 10));
    }

    SECTION("KNN with vector of vectors") {
        const int nSolutions = 10;
        const int dimension = 100;
        const int objs = 1;
        const int k = 3;
        auto solutions = std::vector<std::vector<float>>{};
        for (int i = 0; i < nSolutions; i++) {
            IntDoubleSolution solution(dimension, objs);
            auto vars = std::vector<float>(dimension);
            std::iota(vars.begin(), vars.end(), 0.0f);
            solutions.push_back(vars);
        }
        auto dist = make_unique<Euclidean<float>>();
        auto nearest = KNN().run(solutions, dist.get(), k);
        REQUIRE(nearest.size() == 10);
        // Only gets the k nearest
        for (auto& n : nearest) {
            REQUIRE(n.size() == k);
        }
        // KNN throws runtime error
        REQUIRE_THROWS(KNN().run(solutions, dist.get(), k * 10));
    }
    SECTION("Test the sparseness function equals 0") {
        const int nSolutions = 100;
        const int dimension = 100;
        const int objs = 1;
        const int k = 15;
        vector<IntDoubleSolution> solutions;
        for (int i = 0; i < nSolutions; i++) {
            int begin = 0;
            IntDoubleSolution solution(dimension, objs);
            vector<int> vars(dimension, 0);
            std::generate(vars.begin(), vars.end(), [&]() { return ++begin; });
            solution.setVariables(vars);
            solutions.push_back(solution);
        }
        auto hamming = make_unique<Hamming<int>>();
        vector spars = sparseness(solutions, hamming.get());
        REQUIRE(spars.size() == nSolutions);
        for (const float spar : spars) {
            REQUIRE(abs(spar - 0) < 1e-5);
        }
        // sparseness throws runtime error
        REQUIRE_THROWS(sparseness(solutions, hamming.get(), k * 10));
    }

    SECTION("Test the sparseness function with random pop") {
        const int nSolutions = 10;
        const int dimension = 100;
        const int objs = 1;
        const int k = 3;
        vector<IntDoubleSolution> solutions;
        random_device randomDevice;
        mt19937 randomEngine(randomDevice());
        uniform_int_distribution<int> dist(1, 1000);
        for (int i = 0; i < nSolutions; i++) {
            IntDoubleSolution solution(dimension, objs);
            vector<int> vars(dimension);
            std::generate(vars.begin(), vars.end(),
                          [&] { return dist(randomEngine); });
            solution.setVariables(vars);
            solutions.push_back(solution);
        }
        auto hamming = make_unique<Hamming<int>>();
        vector spars = sparseness(solutions, hamming.get());
        REQUIRE(spars.size() == nSolutions);
        for (const int spar : spars) {
            REQUIRE(spar != 0);
        }
        // sparseness throws runtime error
        REQUIRE_THROWS(sparseness(solutions, hamming.get(), k * 10));
    }

    SECTION("Test the sparseness of one ind") {
        const int nSolutions = 10;
        const int dimension = 100;
        const int objs = 1;
        const int k = 3;
        vector<IntDoubleSolution> solutions;
        for (int i = 0; i < nSolutions; i++) {
            IntDoubleSolution solution(dimension, objs);
            vector<int> vars(dimension);
            std::iota(vars.begin(), vars.end(), 0);
            solution.setVariables(vars);
            solutions.push_back(solution);
        }
        auto hamming = make_unique<Hamming<int>>();
        auto spars = sparseness(solutions[0], solutions, hamming.get());
        REQUIRE(spars == 0);
        // sparseness throws runtime error
        REQUIRE_THROWS(
            sparseness(solutions[0], solutions, hamming.get(), k * 100));
    }
}
