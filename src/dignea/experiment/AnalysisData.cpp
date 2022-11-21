//
// Created by amarrero on 14/1/21.
//

#include "dignea/experiment/AnalysisData.h"

#include <iostream>

using namespace std;

AnalysisData::AnalysisData(const int reps, const double &avgFitness,
                           const double &avgTime,
                           const vector<double> &bestsFitness,
                           const map<int, float> &avgEvolution)
    : repetitions(reps),
      avgFitness(avgFitness),
      averageElapsedTime(avgTime),
      bestFitness(bestsFitness),
      averageEvolution(avgEvolution),
      AVG_OBJ("avg_fitness"),
      AVG_TIME("avg_elapsed_time_s"),
      REPS("repetitions"),
      AVG_EVO("avg_evolution"),
      BEST_OBJS("best_fitness") {}

AnalysisData::~AnalysisData() {}

/**
 * Método que devuelve los resultados obtenidos como un objeto JSON
 * @return
 */
json AnalysisData::to_json() const {
    json results;
    results[REPS] = this->repetitions;
    results[AVG_OBJ] = this->avgFitness;
    results[AVG_TIME] = this->averageElapsedTime;
    results[BEST_OBJS] = this->bestFitness;
    results[AVG_EVO] = this->averageEvolution;
    return results;
}
