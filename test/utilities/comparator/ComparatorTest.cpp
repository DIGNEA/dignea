//
// Created by amarrero on 15/12/20.
//

#include <dignea/problems/SampleProblem.h>
#include <dignea/types/SolutionTypes.h>
#include <dignea/utilities/Comparator.h>

#include <catch2/catch.hpp>

TEST_CASE("Comparator can be created", "[Comparator]") {
    int dimension = 30;
    int numObjectives = 2;
    float minFitness = 50;
    float maxFitness = 100;
    FloatSolution firstInd(dimension, numObjectives);
    FloatSolution secondInd(dimension, numObjectives);
    SECTION("Compare by fitness - First wins") {
        firstInd.setFitness(maxFitness);
        secondInd.setFitness(minFitness);
        REQUIRE(cmpByFitness(firstInd, secondInd) == FIRST_BEST);
    }
    SECTION("Compare by fitness - Second wins") {
        firstInd.setFitness(minFitness);
        secondInd.setFitness(maxFitness);
        REQUIRE(cmpByFitness(firstInd, secondInd) == SECOND_BEST);
    }
    SECTION("Compare by fitness - Tie") {
        firstInd.setFitness(minFitness);
        secondInd.setFitness(minFitness);
        REQUIRE(cmpByFitness(firstInd, secondInd) == TIE);
    }

    SECTION("Compare by objective - First wins") {
        auto sample = make_shared<SampleProblem>(dimension);
        firstInd.setObjAt(0, minFitness);
        secondInd.setObjAt(0, maxFitness);
        REQUIRE(cmpByObj(firstInd, secondInd, 0, sample.get()) == FIRST_BEST);
    }
    SECTION("Compare by fitness - Second wins") {
        auto sample = make_shared<SampleProblem>(dimension);
        firstInd.setObjAt(0, maxFitness);
        secondInd.setObjAt(0, minFitness);
        REQUIRE(cmpByObj(firstInd, secondInd, 0, sample.get()) == SECOND_BEST);
    }
    SECTION("Compare by fitness - Tie") {
        auto sample = make_shared<SampleProblem>(dimension);
        firstInd.setObjAt(0, maxFitness);
        secondInd.setObjAt(0, maxFitness);
        REQUIRE(cmpByObj(firstInd, secondInd, 0, sample.get()) == TIE);
    }

    SECTION("Compare by objective Maximise - Second Wins") {
        auto sample = make_shared<SampleProblem>(dimension);
        firstInd.setObjAt(1, minFitness);
        secondInd.setObjAt(1, maxFitness);
        REQUIRE(cmpByObj(firstInd, secondInd, 1, sample.get()) == SECOND_BEST);
    }

    SECTION("Compare by objective Maximise - First Wins") {
        auto sample = make_shared<SampleProblem>(dimension);
        firstInd.setObjAt(1, maxFitness);
        secondInd.setObjAt(1, minFitness);
        REQUIRE(cmpByObj(firstInd, secondInd, 1, sample.get()) == FIRST_BEST);
    }

    SECTION("Compare by objective Maximise - Tie") {
        auto sample = make_shared<SampleProblem>(dimension);
        firstInd.setObjAt(1, maxFitness);
        secondInd.setObjAt(1, maxFitness);
        REQUIRE(cmpByObj(firstInd, secondInd, 1, sample.get()) == TIE);
    }

    SECTION("Throw exception because outOfRange cmpByObj") {
        auto sample = make_shared<SampleProblem>(dimension);
        REQUIRE_THROWS(
            cmpByObj(firstInd, secondInd, numObjectives + 2, sample.get()));
    }
}