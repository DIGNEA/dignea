//
// Created by amarrero on 13/1/21.
//

#ifndef DIGNEA_EXPERIMENTANALYSIS_H
#define DIGNEA_EXPERIMENTANALYSIS_H

/**
 * ExperimentAnalysis.h
 *
 * Author: Alejandro Marrero
 * Contact: alejandro.marrero.10@ull.edu.es
 *
 *  01 - 2021
 **/
#include <dignea/core/Front.h>
#include <dignea/metrics/BestFitness.h>

#include <map>
#include <nlohmann/json.hpp>
#include <vector>

#include "AnalysisData.h"
#include "dignea/evolution/Evolution.h"
#include "dignea/utilities/Sorter.h"

using namespace std;
using json = nlohmann::json;

// Parejas de resultados y tiempo de ejecucion
using executionResults = std::pair<vector<double>, double>;

template <class S>
class ExperimentAnalysis {
   public:
    explicit ExperimentAnalysis(const int &repetitions);

    virtual ~ExperimentAnalysis();

    void addResults(const Front<S> &solutions, const double &elapsedTime);

    void addResults(const Front<S> &solutions, const double &elapsedTime,
                    const map<int, float> &evolution);

    AnalysisData getAverageResults();

   private:
    BestFitness<S> bestFitness;  // BestFitness Metric
    vector<double>
        fitness;           // Resultados de las ejecuciones --> objetivos (mono)
    vector<double> times;  // Resultados de las ejecuciones --> tiempos
    map<int, float> evolutions;
    // Numero de ejecuciones independientes
    int maxReps;
};

template <class S>
ExperimentAnalysis<S>::ExperimentAnalysis(const int &repetitions)
    : bestFitness(), maxReps(repetitions) {
    fitness.reserve(maxReps);
    times.reserve(maxReps);
    evolutions = {};
}

template <class S>
ExperimentAnalysis<S>::~ExperimentAnalysis() {
    fitness.clear();
    times.clear();
    evolutions.clear();
}

/**
 * Method which includes the results from a repetition of an experiment in
 * the average results
 * @tparam Problem
 * @tparam S
 * @param solutions
 * @param elapsedTime
 */
template <class S>
void ExperimentAnalysis<S>::addResults(const Front<S> &solutions,
                                       const double &elapsedTime) {
    if (!solutions.empty()) {
        fitness.push_back(bestFitness.getBestSolution(solutions).getFitness());
    } else {
        fitness.push_back(0);
    }
    times.push_back(elapsedTime);
}

/**
 * Method which includes the results from a repetition of an experiment in
 * the average results
 * @tparam Problem
 * @tparam S
 * @param solutions
 * @param elapsedTime
 * @param evolution
 */
template <class S>
void ExperimentAnalysis<S>::addResults(const Front<S> &solutions,
                                       const double &elapsedTime,
                                       const map<int, float> &evolution) {
    if (!solutions.empty()) {
        fitness.push_back(bestFitness.getBestSolution(solutions).getFitness());
    } else {
        fitness.push_back(0);
    }
    times.push_back(elapsedTime);

    for (auto &[checkpoint, value] : evolution) {
        if (evolutions.find(checkpoint) != evolutions.end()) {
            evolutions[checkpoint] += value;
        } else {
            evolutions[checkpoint] = value;
        }
    }
}

/**
 * Método que calcula la media de los resultados de las ejecuciones
 * @tparam Problem
 * @tparam S
 * @return
 */
template <class S>
AnalysisData ExperimentAnalysis<S>::getAverageResults() {
    double avgFitness = 0;
    double avgElapsedTime = 0;
    for (const double objective : fitness) {
        avgFitness += objective;
    }
    for (const double time : times) {
        avgElapsedTime += time;
    }
    for (auto &[checkpoint, value] : evolutions) {
        value /= maxReps;
    }
    avgFitness /= maxReps;
    avgElapsedTime /= maxReps;
    return AnalysisData(maxReps, avgFitness, avgElapsedTime, fitness,
                        evolutions);
}

#endif