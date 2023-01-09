//
// Created by amarrero on 10/12/20.
//

#include <dignea/core/Solution.h>

#include <nlohmann/json.hpp>

#include <catch2/catch_all.hpp>

using json = nlohmann::json;

TEST_CASE("Solutions can be created", "[Solution]") {
    int nVars = 30;
    int nObjs = 1;
    int nCons = 2;
    Solution<int, double> solution(nVars, nObjs, nCons);

    SECTION("Creating an empty Solution - VARS") {
        Solution<int, int> emptySolution;
        REQUIRE(emptySolution.getNumberOfVars() == 0);
    }

    SECTION("Creating an empty Solution - Cons") {
        Solution<int, int> emptySolution;
        REQUIRE(emptySolution.getNumberOfCons() == 0);
    }

    SECTION("Creating an empty Solution - Objs") {
        Solution<int, int> emptySolution;
        REQUIRE(emptySolution.getNumberOfObjs() == 0);
    }

    SECTION("Creating an empty Solution - Fitness") {
        Solution<int, int> emptySolution;
        REQUIRE(emptySolution.getFitness() == 0.0);
    }

    SECTION("Creating an empty Solution - Vars are empty") {
        Solution<int, int> emptySolution;
        REQUIRE(emptySolution.getVariables().empty());
    }

    SECTION("Creating an empty Solution - Objs are empty") {
        Solution<int, int> emptySolution;
        REQUIRE(emptySolution.getNumberOfObjs() == 0);
    }

    SECTION("Creating an empty Solution - Const are empty") {
        Solution<int, int> emptySolution;
        REQUIRE(emptySolution.getConstraints().empty());
    }

    SECTION("Copying a Solution - Vars") {
        Solution<int, double> copySolution(solution);
        REQUIRE(copySolution.getNumberOfVars() == solution.getNumberOfVars());
    }

    SECTION("Copying a Solution - Objs") {
        Solution<int, double> copySolution(solution);
        REQUIRE(copySolution.getNumberOfObjs() == solution.getNumberOfObjs());
    }

    SECTION("Copying a Solution - Cons") {
        Solution<int, double> copySolution(solution);
        REQUIRE(copySolution.getNumberOfCons() == solution.getNumberOfCons());
    }

    SECTION("Copying a Solution - Fitness") {
        Solution<int, double> copySolution(solution);
        REQUIRE(copySolution.getFitness() == solution.getFitness());
    }

    SECTION("Copying a Solution - Vars vector") {
        Solution<int, double> copySolution(solution);
        REQUIRE(copySolution.getVariables() == solution.getVariables());
    }

    SECTION("Copying a Solution - Objs vector") {
        Solution<int, double> copySolution(solution);
        REQUIRE(copySolution.getObjectives() == solution.getObjectives());
    }

    SECTION("Copying a Solution - Const vector") {
        Solution<int, double> copySolution(solution);
        REQUIRE(copySolution.getConstraints() == solution.getConstraints());
    }

    SECTION("Accessing members - getNumberOfVars") {
        REQUIRE(solution.getNumberOfVars() == 30);
    }

    SECTION("Accessing members - getNumberOfObjs") {
        REQUIRE(solution.getNumberOfObjs() == 1);
    }

    SECTION("Accessing members - getNumberOfCons") {
        REQUIRE(solution.getNumberOfCons() == 2);
    }

    SECTION("Accessing members - getFitness") {
        REQUIRE(solution.getFitness() == 0.0);
    }

    SECTION("Accessing members - getVarAt 0") {
        REQUIRE(solution.getVarAt(0) == 0);
    }

    SECTION("Accessing members - getVarAt out of range -1") {
        REQUIRE_THROWS(solution.getVarAt(-1));
    }

    SECTION("Accessing members - getVarAt out of range + 10") {
        REQUIRE_THROWS(solution.getVarAt(nVars + 10));
    }

    SECTION("cloning a solution - getNumberOfVars") {
        auto copy = make_unique<Solution<int, double>>(solution);
        REQUIRE(copy->getNumberOfVars() == solution.getNumberOfVars());
    }

    SECTION("cloning a solution - getNumberOfObjs") {
        auto copy = make_unique<Solution<int, double>>(solution);
        REQUIRE(copy->getNumberOfObjs() == solution.getNumberOfObjs());
    }

    SECTION("cloning a solution - getNumberOfCons") {
        auto copy = make_unique<Solution<int, double>>(solution);
        REQUIRE(copy->getNumberOfCons() == solution.getNumberOfCons());
    }

    SECTION("cloning an entire solution") {
        auto copy = make_unique<Solution<int, double>>(&solution);
        REQUIRE(copy->getNumberOfCons() == solution.getNumberOfCons());
        REQUIRE(copy->getNumberOfObjs() == solution.getNumberOfObjs());
        REQUIRE(copy->getNumberOfVars() == solution.getNumberOfVars());
    }

    SECTION("Cloning an entire solution") {
        auto copy = make_unique<Solution<int, double>>(&solution);
        REQUIRE(copy->getNumberOfCons() == solution.getNumberOfCons());
        REQUIRE(copy->getNumberOfObjs() == solution.getNumberOfObjs());
        REQUIRE(copy->getNumberOfVars() == solution.getNumberOfVars());
    }

    SECTION("Comparing two Solutions") {
        Solution<int, double> solution1(nVars, nObjs, nCons);
        Solution<int, double> solution2(nVars, nObjs, nCons);
        solution1.setFitness(100);
        solution2.setFitness(100);
        REQUIRE(solution1 == solution2);
        solution2.setConstraintCoeff(1000);
        REQUIRE_FALSE(solution1 == solution2);
    }

    SECTION("Acessing getters of vectors from solution - Vars") {
        REQUIRE(solution.getVariables().size() == (long unsigned int)nVars);
    }

    SECTION("Acessing getters of vectors from solution - Objs") {
        REQUIRE(solution.getObjectives().size() == (long unsigned int)nObjs);
    }

    SECTION("Acessing getters of vectors from solution - Const") {
        REQUIRE(solution.getConstraints().size() == (long unsigned int)nCons);
    }

    SECTION("Updating the fitness of a solution") {
        double newObjective = 50.0;
        solution.setObjAt(0, newObjective);
        REQUIRE(solution.getObjAt(0) == newObjective);
    }

    SECTION("Getting the fitness of a solution - Going out-of-range + 1") {
        REQUIRE_THROWS(solution.getObjAt(1));
    }

    SECTION("Updating the fitness of a solution - Going out-of-range + 1") {
        double newObjective = 50.0;
        // Cuando accedemos a un objetivo que no existe salta un error
        REQUIRE_THROWS(solution.setObjAt(1, newObjective));
    }

    SECTION("Updating the variables of a solution - Position 0") {
        int newVariable = 25;
        solution.setVarAt(0, newVariable);
        REQUIRE(solution.getVarAt(0) == newVariable);
    }

    SECTION("Updating the variables of a solution - Whole vector") {
        vector<int> variables(nVars);
        iota(variables.begin(), variables.end(), 0);
        // Cuando actualizamos todas las variables
        solution.setVariables(variables);
        REQUIRE(solution.getVariables() == variables);
    }

    SECTION("Updating the variables of a solution - out of range + 2") {
        REQUIRE_THROWS(solution.setVarAt(nVars + 2, 0));
    }

    SECTION("Getting the variables of a solution - out of range + 2") {
        REQUIRE_THROWS(solution.getVarAt(nVars + 2));
    }

    SECTION("Updating the constraints of a solution - Whole vector") {
        vector<double> constraints = {25.0, -50.1};
        solution.setConstraints(constraints);
        REQUIRE(solution.getConstraints() == constraints);
    }

    SECTION("Updating the constraints of a solution - Get 0") {
        vector<double> constraints = {25.0, -50.1};
        solution.setConstraints(constraints);
        REQUIRE(solution.getConstAt(0) == 25.0);
    }

    SECTION("Updating the constraints of a solution - Get 1") {
        vector<double> constraints = {25.0, -50.1};
        solution.setConstraints(constraints);
        REQUIRE(solution.getConstAt(1) == -50.1);
    }

    SECTION(
        "Updating the constraints of a solution - Whole vector out of range "
        "+2") {
        vector<double> constraints = {25.0, -50.1};
        solution.setConstraints(constraints);
        // Cuando accedemos a un objetivo que no existe salta un error
        REQUIRE_THROWS(solution.setConstAt(nCons + 2, constraints[0]));
    }

    SECTION(
        "Getting the constraints of a solution - Whole vector out of range "
        "+2") {
        vector<double> constraints = {25.0, -50.1};
        solution.setConstraints(constraints);
        // Cuando accedemos a un objetivo que no existe salta un error
        REQUIRE_THROWS(solution.getConstAt(nCons + 2));
    }

    SECTION("Using setters of a solution - Vars") {
        const int newVars = 100;
        solution.setNumberOfVars(newVars);
        REQUIRE(solution.getNumberOfVars() == newVars);
    }

    SECTION("Using setters of a solution - Objs") {
        const int newObjs = 10;
        solution.setNumberOfbjs(newObjs);
        REQUIRE(solution.getNumberOfObjs() == newObjs);
    }

    SECTION("Using setters of a solution - Cons") {
        const int newConst = 2;
        solution.setNumberOfCons(newConst);
        REQUIRE(solution.getNumberOfCons() == newConst);
    }

    SECTION("Comparing two solutions - Equal") {
        Solution<int, double> copy(solution);
        bool result = solution == copy;
        REQUIRE(result);
    }

    SECTION("Comparing two solutions - Distinct") {
        Solution<int, double> copy(solution);
        copy.setNumberOfbjs(10);
        bool result = solution == copy;
        REQUIRE_FALSE(result);
    }

    SECTION("Throwing exception when copying null solution") {
        Solution<int, int> *sample = nullptr;
        REQUIRE_THROWS(Solution<int, int>(sample));
    }
}