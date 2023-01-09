

#include <dignea/experiment/AnalysisData.h>
#include <dignea/types/SolutionTypes.h>

#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include <catch2/catch_all.hpp>

using json = nlohmann::json;
using namespace std;

TEST_CASE("AnalysisData Test", "[AnalysisData]") {
    SECTION("Creating an AnalysisData") {
        vector<double> emptyVector = {};
        map<int, float> emptyMap;
        AnalysisData data(10, 10.0, 0.002, emptyVector, emptyMap);
        REQUIRE(data.getRepetitions() == 10);
        REQUIRE(data.getAverageElapsedTime() == 0.002);
        REQUIRE(data.getAvgFitness() == 10.0);
        REQUIRE(data.getBestFitness() == emptyVector);
        REQUIRE(data.getAverageEvolution() == emptyMap);
    }

    SECTION("Setting Repetitions") {
        vector<double> emptyVector = {};
        map<int, float> emptyMap;
        AnalysisData data(10, 10.0, 0.002, emptyVector, emptyMap);
        REQUIRE(data.getRepetitions() == 10);
        data.setRepetitions(100);
        REQUIRE(data.getRepetitions() == 100);
    }

    SECTION("Setting Avg. Fitness") {
        vector<double> emptyVector = {};
        map<int, float> emptyMap;
        AnalysisData data(10, 10.0, 0.002, emptyVector, emptyMap);
        REQUIRE(data.getAvgFitness() == 10.0);
        data.setAvgFitness(100.50);
        REQUIRE(data.getAvgFitness() == 100.50);
    }

    SECTION("Setting Avg. Elapsed Time") {
        vector<double> emptyVector = {};
        map<int, float> emptyMap;
        AnalysisData data(10, 10.0, 0.002, emptyVector, emptyMap);
        REQUIRE(data.getAverageElapsedTime() == 0.002);
        data.setAverageElapsedTime(0.50);
        REQUIRE(data.getAverageElapsedTime() == 0.50);
    }

    SECTION("Setting best fitness") {
        vector<double> emptyVector = {};
        map<int, float> emptyMap;
        AnalysisData data(10, 10.0, 0.002, emptyVector, emptyMap);
        REQUIRE(data.getBestFitness() == emptyVector);
        vector bestFitness = {20.90, 10.0};
        data.setBestFitness(bestFitness);
        REQUIRE(data.getBestFitness() == bestFitness);
    }

    SECTION("Setting average evolution fitness") {
        vector<double> emptyVector = {};
        map<int, float> emptyMap;
        AnalysisData data(10, 10.0, 0.002, emptyVector, emptyMap);
        REQUIRE(data.getAverageEvolution() == emptyMap);
        map<int, float> evolution = {{0, 10}, {1, 10.50}, {2, 40.0}};
        data.setAverageEvolution(evolution);
        REQUIRE(data.getAverageEvolution() == evolution);
    }

    SECTION("AnalysisData to_json") {
        auto reps = 10;
        auto avgFitness = 100.0;
        auto avgTime = 30.0;
        vector<double> bests = {100.0, 100.0, 100.0, 100.0, 100.0,
                                100.0, 100.0, 100.0, 100.0, 100.0};
        map<int, float> avgEvo = {};

        auto data = AnalysisData(reps, avgFitness, avgTime, bests, avgEvo);
        json j = data.to_json();
        REQUIRE(j["repetitions"] == reps);
        REQUIRE(j["avg_fitness"] == avgFitness);
        REQUIRE(j["avg_elapsed_time_s"] == avgTime);
        REQUIRE(j["avg_evolution"] == avgEvo);
        REQUIRE(j["best_fitness"] == bests);
    }
}