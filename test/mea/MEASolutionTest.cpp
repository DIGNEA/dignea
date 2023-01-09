//
// Created by amarrero on 15/4/21.
//

#include <dignea/mea/MEASolution.h>
#include <dignea/types/SolutionTypes.h>

#include <iostream>  // std::cout
#include <nlohmann/json.hpp>
#include <sstream>  // std::stringstream
#include <string>   // std::string
#include <vector>

#include <catch2/catch_all.hpp>

using json = nlohmann::json;

using namespace std;

TEST_CASE("MEASolution tests", "[MEASolution]") {
    SECTION("Creating a MEASolution object") {
        auto solution = make_unique<MEASolution<int, int>>();
        REQUIRE(solution);
        REQUIRE(solution->getNumberOfVars() == 0);
        REQUIRE(solution->getNumberOfObjs() == 0);
        REQUIRE(solution->getNumberOfCons() == 0);
    }

    SECTION("Creating a MEASolution with the vars") {
        auto solution = make_unique<MEASolution<int, int>>(100);
        REQUIRE(solution);
        REQUIRE(solution->getNumberOfVars() == 100);
        REQUIRE(solution->getNumberOfObjs() == 1);
        REQUIRE(solution->getNumberOfCons() == 0);
    }

    SECTION("Creating a MEASolution with the vars and objs") {
        auto solution = make_unique<MEASolution<int, int>>(100, 1);
        REQUIRE(solution);
        REQUIRE(solution->getNumberOfVars() == 100);
        REQUIRE(solution->getNumberOfObjs() == 1);
        REQUIRE(solution->getNumberOfCons() == 0);

        solution = make_unique<MEASolution<int, int>>(100, 2);
        REQUIRE(solution);
        REQUIRE(solution->getNumberOfVars() == 100);
        REQUIRE(solution->getNumberOfObjs() == 2);
        REQUIRE(solution->getNumberOfCons() == 0);
    }

    SECTION("Creating a MEASolution with the vars, objs and cons") {
        auto solution = make_unique<MEASolution<int, int>>(100, 1);
        REQUIRE(solution);
        REQUIRE(solution->getNumberOfVars() == 100);
        REQUIRE(solution->getNumberOfObjs() == 1);
        REQUIRE(solution->getNumberOfCons() == 0);

        solution = make_unique<MEASolution<int, int>>(100, 2);
        REQUIRE(solution);
        REQUIRE(solution->getNumberOfVars() == 100);
        REQUIRE(solution->getNumberOfObjs() == 2);
        REQUIRE(solution->getNumberOfCons() == 0);

        solution = make_unique<MEASolution<int, int>>(100, 1, 1);
        REQUIRE(solution);
        REQUIRE(solution->getNumberOfVars() == 100);
        REQUIRE(solution->getNumberOfObjs() == 1);
        REQUIRE(solution->getNumberOfCons() == 1);

        solution = make_unique<MEASolution<int, int>>(100, 2);
        REQUIRE(solution);
        REQUIRE(solution->getNumberOfVars() == 100);
        REQUIRE(solution->getNumberOfObjs() == 2);
        REQUIRE(solution->getNumberOfCons() == 0);
    }

    SECTION("Creating a MEASolution from other using assignment operators") {
        auto solution = MEASolution<int, int>(100, 2);
        REQUIRE(solution.getNumberOfVars() == 100);
        REQUIRE(solution.getNumberOfObjs() == 2);
        REQUIRE(solution.getNumberOfCons() == 0);

        auto other = solution;
        REQUIRE(other.getNumberOfVars() == 100);
        REQUIRE(other.getNumberOfObjs() == 2);
        REQUIRE(other.getNumberOfCons() == 0);
        REQUIRE(other.getFitness() == solution.getFitness());
        REQUIRE(other.getBiasedFitness() == solution.getBiasedFitness());
        REQUIRE(other.getDiversity() == solution.getDiversity());
        REQUIRE(other.getFeatures() == solution.getFeatures());
        REQUIRE(solution == other);
    }

    SECTION("Comparing two  MEASolutions") {
        auto solution = MEASolution<int, int>(100, 2);
        auto other = MEASolution<int, int>(100, 2);
        REQUIRE(solution == other);
    }

    SECTION("Creating a MEASolution with other") {
        auto solution = make_unique<MEASolution<int, int>>(100, 1, 1);
        auto other = make_unique<MEASolution<int, int>>(100, 2);

        REQUIRE(solution);
        REQUIRE(solution->getNumberOfVars() == 100);
        REQUIRE(solution->getNumberOfObjs() == 1);
        REQUIRE(solution->getNumberOfCons() == 1);

        REQUIRE(other);
        REQUIRE(other->getNumberOfVars() == 100);
        REQUIRE(other->getNumberOfObjs() == 2);
        REQUIRE(other->getNumberOfCons() == 0);

        solution = make_unique<MEASolution<int, int>>(other.get());
        REQUIRE(solution);
        REQUIRE(solution->getNumberOfVars() == other->getNumberOfVars());
        REQUIRE(solution->getNumberOfObjs() == other->getNumberOfObjs());
        REQUIRE(solution->getNumberOfCons() == other->getNumberOfCons());
    }

    SECTION("Setting features") {
        auto solution = make_unique<MEASolution<int, int>>(100, 1, 1);
        vector<float> features = {1.3, 2.0, 3.7, 4.6, 5.8};
        REQUIRE(solution);
        REQUIRE(solution->getNumberOfVars() == 100);
        REQUIRE(solution->getNumberOfObjs() == 1);
        REQUIRE(solution->getNumberOfCons() == 1);
        solution->setFeatures(features);
        REQUIRE(solution->getFeatures() == features);
    }

    SECTION("MEASolution setBiasedFitness") {
        auto solution = make_unique<MEASolution<int, int>>(100, 1, 1);
        auto f = 1000;
        solution->setBiasedFitness(f);
        REQUIRE(solution->getBiasedFitness() == f);
    }

    SECTION("MEASolution setConfigFitness") {
        auto solution = make_unique<MEASolution<int, int>>(100, 1, 1);
        vector<vector<float>> fitness{{100.0f, 100.0f, 0.0f, 10000.0f},
                                      {100.0f, 100.0f, 0.0f, 10000.0f},
                                      {100.0f, 100.0f, 0.0f, 10000.0f}};
        solution->setConfigFitness(fitness);
        REQUIRE(solution->getPortFitness() == fitness);
    }

    SECTION("MEASolution to_json") {
        auto solution = make_unique<MEASolution<int, int>>(4, 1, 1);
        solution->setObjectives({10});
        solution->setConstraints({0});
        solution->setVariables({1, 1, 1, 1});
        auto j = solution->to_json();
        REQUIRE(j.empty());
    }

    SECTION("MEASolution to_instance") {
        std::string expected = "Not implemented in MEASolution";
        std::stringstream buffer;
        std::streambuf* prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());
        auto solution = make_unique<MEASolution<int, int>>(4, 1, 1);
        solution->to_instance(std::cout);
        std::string result = buffer.str();
        REQUIRE(result.compare(expected));
        std::cout.rdbuf(prevcoutbuf);
    }

    SECTION("MEASolution operator<<") {
        std::string expected = "Not implemented in MEASolution";
        std::stringstream buffer;
        std::streambuf* prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());
        auto solution = MEASolution<int, int>(4, 1, 1);
        std::cout << solution;
        std::string result = buffer.str();
        REQUIRE(result.compare(expected));
        std::cout.rdbuf(prevcoutbuf);
    }
}