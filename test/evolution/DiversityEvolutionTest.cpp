

#include <dignea/distances/Distance.h>
#include <dignea/distances/Hamming.h>
#include <dignea/evolution/DiversityEvolution.h>
#include <dignea/types/SolutionTypes.h>

#include <map>
#include <vector>

#include <catch2/catch_all.hpp>
using namespace std;

TEST_CASE("DiversityEvolution Test", "[DiversityEvolution]") {
    SECTION("Creating a empty DiversityEvolution") {
        auto diversity =
            make_unique<Diversity<float>>(make_unique<Hamming<float>>());
        auto divEv = make_unique<DiversityEvolution<float, FloatSolution>>(
            move(diversity));
        REQUIRE_NOTHROW(divEv->results());
        REQUIRE(divEv->results().empty());
    }

    SECTION("Including solutions in DiversityEvolution") {
        auto diversity =
            make_unique<Diversity<float>>(make_unique<Hamming<float>>());
        auto divEv = make_unique<DiversityEvolution<float, FloatSolution>>(
            move(diversity));
        const int nSolutions = 100;
        for (int i = 0; i < nSolutions; i++) {
            FloatSolution solution(100, 1, 0);
            solution.setFitness(float(i));
            vector tmpVector = {solution};
            divEv->update(i, tmpVector);
        }
        REQUIRE_NOTHROW(divEv->results());
    }

    SECTION(
        "Computing DiversityEvolution solutions in DiversityEvolution equals "
        "0") {
        auto diversity =
            make_unique<Diversity<float>>(make_unique<Hamming<float>>());
        auto divEv = make_unique<DiversityEvolution<float, FloatSolution>>(
            move(diversity));
        const int nSolutions = 100;
        const int dim = 100;
        map<int, float> divResults;
        for (int i = 0; i < nSolutions; i++) {
            FloatSolution solution(dim, 1, 0);
            solution.setFitness(float(i));
            vector<float> vars(dim, 0.0);
            solution.setVariables(vars);
            vector tmpVector = {solution};
            divEv->update(i, tmpVector);
            divResults.insert({i, 0});
        }
        REQUIRE_NOTHROW(divEv->results());
        REQUIRE(divResults == divEv->results());
    }
    SECTION(
        "Computing DiversityEvolution solutions in DiversityEvolution != 0") {
        auto diversity =
            make_unique<Diversity<float>>(make_unique<Hamming<float>>());
        auto divEv = make_unique<DiversityEvolution<float, FloatSolution>>(
            move(diversity));
        const int checkpoints = 100;
        const int nSolutions = 10;
        const int dim = 100;
        map<int, float> divResults;

        for (int check = 0; check < checkpoints; check++) {
            vector<FloatSolution> solutions;
            solutions.reserve(nSolutions);
            for (int i = 0; i < nSolutions; i++) {
                FloatSolution solution(dim, 1, 0);
                solution.setFitness(float(i));
                vector<float> vars(dim, 100.0);
                solution.setVariables(vars);
                solutions.push_back(solution);
            }
            divEv->update(check, solutions);
            float div = divEv->getDivMetric()->compute(solutions);
            divResults.insert({check, div});
        }
        REQUIRE_NOTHROW(divEv->results());
        REQUIRE(divResults == divEv->results());
    }
}