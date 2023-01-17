//
// Created by amarrero on 5/7/21.
//

#include <dignea/distances/Euclidean.h>
#include <dignea/generator/domains/KPDomain.h>
#include <dignea/generator/instances/KPInstance.h>
#include <dignea/searches/NSFeatures.h>

#include <algorithm>
#include <nlohmann/json.hpp>
#include <random>
#include <vector>

#include <catch2/catch_all.hpp>

TEST_CASE("NSFeatures Searchs tests", "[NSFeatures Search]") {
    auto dist = make_unique<Euclidean<float>>();
    auto iters{100};
    auto ns = make_unique<NSFeatures<KPInstance>>(move(dist), iters);

    SECTION("Check NS-EIG run") {
        auto pSize = 50;
        auto nVars = 100;
        auto problem = make_unique<KPDomain>(nVars);
        auto inds = std::vector<KPInstance>{};
        for (int i = 0; i < pSize; i++) {
            inds.push_back(problem->createSolution());
        }
        vector result = ns->run(inds, problem.get());
        REQUIRE(result == inds);
        for (auto &ind : inds) {
            REQUIRE(ind.getDiversity() == 0);
        }
    }

    SECTION("Check NS-EIG inserts") {
        auto solution = KPInstance{100};
        ns->insertIntoArchive(solution);
        REQUIRE(1 == ns->getArchive().size());
    }

    SECTION("Check NS-EIG to_json") {
        using json = nlohmann::json;
        json data = ns->to_json();
        REQUIRE("NSFeatures" == data["name"]);
        REQUIRE(2000 == data["threshold"]);
        REQUIRE(15 == data["k"]);
    }
}