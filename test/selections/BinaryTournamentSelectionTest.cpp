//
// Created by amarrero on 21/12/20.
//

#include "catch2/catch.hpp"
#include <dignea/selections/BinaryTournamentSelection.h>
#include <dignea/problems/Sphere.h>
#include <vector>

using namespace std;

TEST_CASE("Testing BinaryTournamentSelection", "[BinaryTournamentSelection]") {
    int dimension = 10;
    int populationSize = 32;
    shared_ptr<Problem<FloatSolution>> sphere = make_shared<Sphere>(dimension);
    vector<FloatSolution> population;
    population.reserve(populationSize);
    // Creamos una poblacion para realizar la seleccion
    for (int i = 0; i < populationSize; i++) {
        population.push_back(sphere->createSolution());
    }

    unique_ptr<Selection<FloatSolution>> selection =
            make_unique<BinaryTournamentSelection<FloatSolution>>();
    SECTION("BinaryTournamentSelection returns unique_ptr Solution") {
        // Ejecutamos la seleccion binaria
        FloatSolution parent = selection->select(population);
        REQUIRE_FALSE((&parent) == nullptr);
        // Comprobamos que no hemos dejado un nulo en la poblacion
        // sino que hemos realizado una copia de la solucion
        for (FloatSolution &solution : population) {
            REQUIRE_FALSE((&solution) == nullptr);
        }
    }

    SECTION("BinaryTournamentSelection returns integer with position Solution") {
        // Ejecutamos la seleccion binaria
        int position = -1;
        selection->select(population, position);
        REQUIRE_FALSE(position == -1);
        // Comprobamos que no hemos dejado un nulo en la poblacion
        // sino que hemos realizado una copia de la solucion
        for (FloatSolution &solution : population) {
            REQUIRE_FALSE((&solution) == nullptr);
        }
    }

    SECTION("Binary Tournament Selection getName") {

        REQUIRE(selection->getName() == "Binary Tournament Selection");
    }
}