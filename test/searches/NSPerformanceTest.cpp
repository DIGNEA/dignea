//
// Created by amarrero on 5/7/21.
//

#include <dignea/distances/Euclidean.h>
#include <dignea/generator/domains/KPDomain.h>
#include <dignea/generator/instances/KPInstance.h>
#include <dignea/searches/NSPerformance.h>

#include <algorithm>
#include <catch2/catch_all.hpp>
#include <nlohmann/json.hpp>
#include <random>
#include <vector>

TEST_CASE("NSPerformance Searchs tests", "[NSPerformance Search]") {
    auto threshold = 30;
    auto pSize = 50;
    auto nVars = 100;
    auto portfolioSize = 4;
    auto problem = make_unique<KPDomain>(nVars);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 500.0);

    auto inds = std::vector<KPInstance>{};
    for (int i = 0; i < pSize; i++) {
        auto solution = problem->createSolution();
        vector<float> fitness(portfolioSize);
        std::generate(fitness.begin(), fitness.end(),
                      [&]() { return dis(gen); });
        solution.setAvgConfigFitness(fitness);
        inds.push_back(solution);
    }

    SECTION("Check NSp-EIG run can calculate diversity") {
        auto dist = make_unique<Euclidean<float>>();
        auto ns =
            make_unique<NSPerformance<KPInstance>>(move(dist), threshold, 3);
        vector result = ns->run(inds, problem.get());
        REQUIRE(result == inds);
        long unsigned int inserted = 0;
        for (auto &ind : inds) {
            REQUIRE(ind.getDiversity() != 0);
            if (ind.getDiversity() > threshold) {
                ns->insertIntoArchive(ind);
                inserted++;
            }
        }
        REQUIRE(ns->getArchive().size() ==
                inserted);  // Some solutions should've been
                            // inserted into the archive
    }

    SECTION("Check NSp-EIG inserts") {
        auto dist = make_unique<Euclidean<float>>();
        auto ns =
            make_unique<NSPerformance<KPInstance>>(move(dist), threshold, 1);
        auto solution = KPInstance{100};
        solution.setAvgConfigFitness({100, 0, 10, 0});
        ns->insertIntoArchive(solution);
        REQUIRE(1 == ns->getArchive().size());
    }

    SECTION("Check NSp-EIG to_json") {
        auto dist = make_unique<Euclidean<float>>();
        auto ns = make_unique<NSPerformance<KPInstance>>(move(dist));
        using json = nlohmann::json;
        json data = ns->to_json();
        REQUIRE("NSPerformance" == data["name"]);
        REQUIRE(2000 == data["threshold"]);
        REQUIRE(15 == data["k"]);
    }
}