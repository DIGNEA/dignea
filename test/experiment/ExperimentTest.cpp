//
// Created by amarrero on 14/4/21.
//

#include <dignea/algorithms/singleobjective/GenerationalGA.h>
#include <dignea/algorithms/singleobjective/SteadyGA.h>
#include <dignea/builders/GABuilder.h>
#include <dignea/experiment/Experiment.h>
#include <dignea/problems/Sphere.h>

#include <nlohmann/json.hpp>
#include <string>

#include <catch2/catch_all.hpp>

using json = nlohmann::json;
using namespace std;

TEST_CASE("Experiment tests", "[Experiment]") {
    std::cout.setstate(std::ios_base::failbit);
    std::cerr.setstate(std::ios_base::failbit);

    using AEAPtr = unique_ptr<AbstractSolver<FloatSolution>>;

    SECTION("Creating an Experiment") {
        AEAPtr gga = make_unique<GenerationalGA<FloatSolution>>();
        auto problem = make_unique<Sphere>(100);
        string experimentName = "MyExperiment";
        int repetitions = 10;
        auto experiment = make_unique<Experiment<FloatSolution>>(
            move(problem), move(gga), experimentName, repetitions);
        REQUIRE(experiment != nullptr);
    }

    SECTION("Get Problem to Solve from Experiment") {
        AEAPtr gga = make_unique<GenerationalGA<FloatSolution>>();
        auto problem = make_unique<Sphere>(100);
        string experimentName = "MyExperiment";

        int repetitions = 10;
        auto experiment = make_unique<Experiment<FloatSolution>>(
            move(problem), move(gga), experimentName, repetitions);
        REQUIRE(experiment != nullptr);
        REQUIRE(experiment->getProblemToSolve());
    }

    SECTION("Set Problem to Solve to Experiment") {
        AEAPtr gga = make_unique<GenerationalGA<FloatSolution>>();
        auto problem = make_unique<Sphere>(100);
        string experimentName = "MyExperiment";
        int repetitions = 10;
        auto experiment = make_unique<Experiment<FloatSolution>>(
            move(problem), move(gga), experimentName, repetitions);
        REQUIRE(experiment != nullptr);
        auto newProblem = make_unique<Sphere>(10);
        experiment->setProblemToSolve(move(newProblem));
        REQUIRE(experiment->getProblemToSolve());
    }

    SECTION("Get Algorithm from Experiment") {
        AEAPtr gga = make_unique<GenerationalGA<FloatSolution>>();
        auto problem = make_unique<Sphere>(100);
        string experimentName = "MyExperiment";
        int repetitions = 10;
        auto experiment = make_unique<Experiment<FloatSolution>>(
            move(problem), move(gga), experimentName, repetitions);
        REQUIRE(experiment != nullptr);
        auto gga2 = make_unique<GenerationalGA<FloatSolution>>();
        REQUIRE(experiment->getAlgorithm()->getName() == gga2->getName());
    }

    SECTION("Set Algorithm to Experiment") {
        AEAPtr gga = make_unique<GenerationalGA<FloatSolution>>();
        AEAPtr steady = make_unique<SteadyGA<FloatSolution>>();
        auto problem = make_unique<Sphere>(100);
        string experimentName = "MyExperiment";
        int repetitions = 10;
        auto experiment = make_unique<Experiment<FloatSolution>>(
            move(problem), move(gga), experimentName, repetitions);
        REQUIRE(experiment != nullptr);
        experiment->setAlgorithm(move(steady));
        auto steady2 = make_unique<SteadyGA<FloatSolution>>();

        REQUIRE(experiment->getAlgorithm()->getName() == steady2->getName());
    }

    SECTION("Get Repetitions from Experiment") {
        AEAPtr gga = make_unique<GenerationalGA<FloatSolution>>();
        auto problem = make_unique<Sphere>(100);
        string experimentName = "MyExperiment";
        int repetitions = 10;
        auto experiment = make_unique<Experiment<FloatSolution>>(
            move(problem), move(gga), experimentName, repetitions);
        REQUIRE(experiment != nullptr);
        REQUIRE(experiment->getRepetitions() == repetitions);
    }

    SECTION("Set Repetitions to Experiment") {
        AEAPtr gga = make_unique<GenerationalGA<FloatSolution>>();
        auto problem = make_unique<Sphere>(100);
        string experimentName = "MyExperiment";
        int repetitions = 10;
        auto experiment = make_unique<Experiment<FloatSolution>>(
            move(problem), move(gga), experimentName, repetitions);
        REQUIRE(experiment != nullptr);
        const int newReps = 30;
        experiment->setRepetitions(newReps);
        REQUIRE(experiment->getRepetitions() == newReps);
    }

    SECTION("Get Experiment Name from Experiment") {
        AEAPtr gga = make_unique<GenerationalGA<FloatSolution>>();
        auto problem = make_unique<Sphere>(100);
        string experimentName = "MyExperiment";
        int repetitions = 10;
        auto experiment = make_unique<Experiment<FloatSolution>>(
            move(problem), move(gga), experimentName, repetitions);
        REQUIRE(experiment != nullptr);
        REQUIRE(experiment->getName() == experimentName);
    }

    SECTION("Set ExperimentName to Experiment") {
        AEAPtr gga = make_unique<GenerationalGA<FloatSolution>>();
        auto problem = make_unique<Sphere>(100);
        string experimentName = "MyExperiment";
        int repetitions = 10;
        auto experiment = make_unique<Experiment<FloatSolution>>(
            move(problem), move(gga), experimentName, repetitions);
        REQUIRE(experiment != nullptr);
        const string newName = "MyNewExperimentName";
        experiment->setName(newName);
        REQUIRE(experiment->getName() == newName);
    }

    SECTION("Experiment run") {
        auto problem = make_unique<Sphere>(100);
        unique_ptr<AbstractGA<FloatSolution>> ga =
            GABuilder<FloatSolution>::create(GAType::Generational)
                .with()
                .populationOf(8)
                .with()
                .mutation(MutType::UniformAll)
                .crossover(CXType::Uniform)
                .selection(SelType::Binary)
                .withMutRate(0.05)
                .withCrossRate(0.8)
                .runDuring(10);

        CHECK(ga);
        string experimentName = "MyExperiment";
        int repetitions = 1;
        auto experiment = make_unique<Experiment<FloatSolution>>(
            move(problem), move(ga), experimentName, repetitions);
        REQUIRE(experiment != nullptr);
        REQUIRE(experiment->run());  // Return boolean eq true

        // @brief Uses the previous run to test the json format
        SECTION("Experiment to_json format") {
            json j = experiment->to_json();
            REQUIRE(j["name"] == experimentName);
            REQUIRE(j["problem"] == experiment->getProblemToSolve()->to_json());
            REQUIRE(j["algorithm"] == experiment->getAlgorithm()->to_json());
            REQUIRE(j["reps_results"].is_array());
            REQUIRE(j["bests"].is_array());
        }
    }
}
