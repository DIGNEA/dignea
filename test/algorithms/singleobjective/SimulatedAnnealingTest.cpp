//
// Created by amarrero on 26/6/21.
//

#include <dignea/algorithms/singleobjective/SimulatedAnnealing.h>
#include <dignea/core/Front.h>
#include <dignea/crossovers/UniformCrossover.h>
#include <dignea/mutations/UniformOneMutation.h>
#include <dignea/problems/SampleProblem.h>
#include <dignea/problems/Sphere.h>
#include <dignea/selections/BinaryTournamentSelection.h>

#include <memory>
#include <typeinfo>
#include <vector>

#include "catch2/catch.hpp"

using namespace std;

TEST_CASE("Simulated Annealing Algorithm Tests", "[SimulatedAnnealing]") {
  int dimension = 10;
  int evals = 1000;
  float initialTemp = 2000.0;
  float tempVar = 0.8;

  SECTION("Default creation of SimulatedAnnealing") {
    unique_ptr<SimulatedAnnealing<FloatSolution>> sa =
        make_unique<SimulatedAnnealing<FloatSolution>>(evals, initialTemp,
                                                       tempVar);
    REQUIRE_FALSE(sa == nullptr);
    REQUIRE(sa->getPopulationSize() == 1);
    REQUIRE(sa->getMaxEvaluations() == evals);
    REQUIRE(sa->getProblem() == nullptr);
  }

  SECTION("Getting SA-ID") {
    unique_ptr<SimulatedAnnealing<FloatSolution>> sa =
        make_unique<SimulatedAnnealing<FloatSolution>>(evals, initialTemp,
                                                       tempVar);
    REQUIRE(sa->getID() == "SA");
  }

  SECTION("Setting maxEvaluations") {
    unique_ptr<SimulatedAnnealing<FloatSolution>> sa =
        make_unique<SimulatedAnnealing<FloatSolution>>(evals, initialTemp,
                                                       tempVar);
    sa->setMaxEvaluations(evals);
    REQUIRE(sa->getMaxEvaluations() == evals);
  }

  SECTION("Setting populationSize") {
    unique_ptr<SimulatedAnnealing<FloatSolution>> sa =
        make_unique<SimulatedAnnealing<FloatSolution>>(evals, initialTemp,
                                                       tempVar);
    REQUIRE_THROWS(sa->setPopulationSize(100));
  }

  SECTION("Setting a Multi-Objective Problem with raw pointer") {
    unique_ptr<SimulatedAnnealing<FloatSolution>> sa =
        make_unique<SimulatedAnnealing<FloatSolution>>(evals, initialTemp,
                                                       tempVar);
    auto sample = make_unique<SampleProblem>(dimension);
    REQUIRE_THROWS(sa->setProblem(sample.get()));
  }

  SECTION("Setting a Multi-objective problem") {
    unique_ptr<SimulatedAnnealing<FloatSolution>> sa =
        make_unique<SimulatedAnnealing<FloatSolution>>(evals, initialTemp,
                                                       tempVar);
    unique_ptr<Problem<FloatSolution>> sample =
        make_unique<SampleProblem>(dimension);
    REQUIRE_THROWS(sa->setProblem(move(sample)));
  }

  SECTION("Creating a SA sa with a Multi-objective problem") {
    auto sample = make_unique<Sphere>(100);
    sample->setNumberOfObjs(2);
    unique_ptr<SimulatedAnnealing<FloatSolution>> sa =
        make_unique<SimulatedAnnealing<FloatSolution>>(evals, initialTemp,
                                                       tempVar);
    REQUIRE_THROWS(sa->setProblem(move(sample)));
  }

  SECTION("Creating SimulatedAnnealing with Problem and evals") {
    auto sphere = make_unique<Sphere>(dimension);
    unique_ptr<SimulatedAnnealing<FloatSolution>> sa =
        make_unique<SimulatedAnnealing<FloatSolution>>(evals, initialTemp,
                                                       tempVar);
    sa->setProblem(move(sphere));
    sa->setMaxEvaluations(evals);
    REQUIRE(sa->getPopulationSize() == 1);
    REQUIRE(sa->getMaxEvaluations() == evals);
    REQUIRE(sa->getProblem()->getName() == "Sphere");
  }

  SECTION("Running SimulatedAnnealing without problem") {
    unique_ptr<SimulatedAnnealing<FloatSolution>> sa =
        make_unique<SimulatedAnnealing<FloatSolution>>(evals, initialTemp,
                                                       tempVar);
    REQUIRE_THROWS(sa->run());
  }

  SECTION("SimulatedAnnealing runs perfectly") {
    auto sa = make_unique<SimulatedAnnealing<FloatSolution>>(evals, initialTemp,
                                                             tempVar);
    auto sphere = make_unique<Sphere>(dimension);
    sa->setProblem(move(sphere));
    sa->run();
    REQUIRE(sa->getPerformedEvaluations() >= sa->getMaxEvaluations());
    REQUIRE_FALSE(sa->getElapsedTime() == 0.0);
    Front<FloatSolution> solutions = sa->getResults();
    REQUIRE(solutions.getNumOfSolutions() == 1);
    REQUIRE(solutions.getSolutions()[0].getFitness() == 0);
  }
}