//
// Created by amarrero on 18/3/21.
//

#include <dignea/problems/SampleProblem.h>

#include <catch2/catch_all.hpp>

TEST_CASE("SampleProblem Testing", "[SampleProblem]") {
    SECTION("Creating a default SampleProblem") {
        auto *sample = new SampleProblem();
        REQUIRE(sample != nullptr);
        delete (sample);
    }

    SECTION("Creating a SampleProblem with dimension") {
        const int dimension = 10;
        SampleProblem sample(dimension);
        REQUIRE(sample.getNumberOfVars() == dimension);
        REQUIRE(sample.getNumberOfObjs() == 2);
        REQUIRE(sample.getNumberOfCons() == 0);
    }

    SECTION("GetName") {
        SampleProblem sample(10);
        REQUIRE(sample.getName() == "SampleProblem");
    }

    SECTION("Limits") {
        SampleProblem sample(10);
        REQUIRE(sample.getLowerLimit(0) == -1);
        REQUIRE(sample.getUpperLimit(0) == -1);
    }

    SECTION("Create solution") {
        SampleProblem sample(100);
        ParallelPRNG random;
        unique_ptr<FloatSolution> solution =
            make_unique<FloatSolution>(sample.createSolution());
        unique_ptr<FloatSolution> solutionRandom =
            make_unique<FloatSolution>(sample.createSolution(random));
        REQUIRE(solution.get() != nullptr);
        REQUIRE(solutionRandom.get() != nullptr);
    }

    SECTION("Evaluate not implemented") {
        SampleProblem sample(100);
        FloatSolution solution = sample.createSolution();
        REQUIRE_THROWS(sample.evaluate(solution));
    }

    SECTION("EvaluateConstraints not implemented") {
        SampleProblem sample(100);
        FloatSolution solution = sample.createSolution();
        REQUIRE_THROWS(sample.evaluateConstraints(solution));
    }

    SECTION("ReadData not implemented") {
        SampleProblem sample(100);
        REQUIRE_THROWS(sample.readData("notPath"));
    }

    SECTION("Cloning default") {
        auto sample = make_unique<SampleProblem>(100);
        auto copySample = make_unique<SampleProblem>(sample.get());
        REQUIRE(copySample != nullptr);
        REQUIRE(sample != nullptr);
        REQUIRE(copySample->getName() == sample->getName());
    }
}