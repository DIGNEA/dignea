//
// Created by amarrero on 5/7/21.
//

#include <dignea/distances/Euclidean.h>
#include <dignea/generator/domains/KPDomain.h>
#include <dignea/generator/instances/KPInstance.h>
#include <dignea/searches/NoveltySearch.h>

#include <algorithm>
#include <catch2/catch_all.hpp>
#include <nlohmann/json.hpp>
#include <random>
#include <vector>

TEST_CASE("Novelty Searchs tests", "[Novelty Search]") {
    auto dist = make_unique<Euclidean<float>>();
    auto ns = make_unique<NoveltySearch<KPInstance>>(move(dist));

    SECTION("NS default parameters") {
        REQUIRE(ns->getK() == 15);
        REQUIRE(ns->getThreshold() == 2000);
        REQUIRE(ns->getFinalThresh() == 0.0001f);
    }

    SECTION("Check NS run") {
        auto pSize = 50;
        auto defaultNS = -1.0f;
        auto nVars = 100;
        auto problem = make_unique<KPDomain>(nVars);
        auto inds = std::vector<KPInstance>{};
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
        auto problem = make_unique<KPDomain>(nVars);
        auto inds = std::vector<KPInstance>{};
        for (int i = 0; i < pSize; i++) {
            KPInstance ind = problem->createSolution();
            ns->insertIntoArchive(ind);
            inds.push_back(ind);
        }
        REQUIRE(ns->getArchive() == inds);
    }

    SECTION("Check NS compare final throws") {
        vector<KPInstance> emptySols;
        REQUIRE_THROWS(ns->cmpFinals(emptySols));
    }

    SECTION("Check NS insertFinal works") {
        auto pSize = 50;
        auto nVars = 100;
        auto problem = make_unique<KPDomain>(nVars);
        auto inds = std::vector<KPInstance>{};
        // FinalsS should be empty
        // so at least the first solution should be inserted
        for (int i = 0; i < pSize; i++) {
            KPInstance ind = problem->createSolution();
            ind.setFitness(1.0f);
            ind.setBiasedFitness(1.0f);
            inds.push_back(ind);
        }
        REQUIRE_NOTHROW(ns->cmpFinals(inds));
        REQUIRE(ns->getResults().getNumOfSolutions() >= 1);
    }

    SECTION("Check NS to_json") {
        using json = nlohmann::json;
        json data = ns->to_json();
        REQUIRE(data["name"] == "Novelty Search");
        REQUIRE(2000 == data["threshold"]);
        REQUIRE(15 == data["k"]);
    }
}