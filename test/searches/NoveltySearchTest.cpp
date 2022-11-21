//
// Created by amarrero on 5/7/21.
//

#include <dignea/distances/Euclidean.h>
#include <dignea/mea/problems/IKPProblem.h>
#include <dignea/mea/solutions/IKPSolution.h>
#include <dignea/searches/NoveltySearch.h>

#include <algorithm>
#include <nlohmann/json.hpp>
#include <random>
#include <vector>

#include "catch2/catch.hpp"

TEST_CASE("Novelty Searchs tests", "[Novelty Search]") {
    auto dist = make_unique<Euclidean<float>>();
    auto iters{100};
    auto ns = make_unique<NoveltySearch<IKPSolution, float>>(move(dist), iters);

    SECTION("Check NS run") {
        auto pSize = 50;
        auto defaultNS = -1.0f;
        auto nVars = 100;
        auto problem = make_unique<IKPProblem>(nVars);
        auto inds = std::vector<IKPSolution>{};
        for (int i = 0; i < pSize; i++) {
            auto s = problem->createSolution();
            s.setDiversity(defaultNS);
            inds.push_back(s);
        }
        vector result = ns->run(inds, problem.get());
        REQUIRE(result == inds);
        for (auto &ind : inds) {
            REQUIRE(ind.getDiversity() != defaultNS);
        }
    }

    SECTION("NS can insert into archive") {
        auto pSize = 50;
        auto nVars = 100;
        auto problem = make_unique<IKPProblem>(nVars);
        auto inds = std::vector<IKPSolution>{};
        for (int i = 0; i < pSize; i++) {
            IKPSolution ind = problem->createSolution();
            ns->insertIntoArchive(ind);
            inds.push_back(ind);
        }
        REQUIRE(ns->getArchive() == inds);
    }

    SECTION("Check NS compare final throws") {
        vector<IKPSolution> emptySols(0);

        // TODO  REQUIRE_THROWS(ns->cmpFinals(emptySols));
    }

    SECTION("Check NS to_json") {
        using json = nlohmann::json;
        json data = ns->to_json();
        REQUIRE(data["name"] == "Novelty Search");
        REQUIRE(2000 == data["threshold"]);
        REQUIRE(15 == data["k"]);
    }
}