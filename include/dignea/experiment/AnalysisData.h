//
// Created by amarrero on 14/1/21.
//

#ifndef DIGNEA_ANALYSISDATA_H
#define DIGNEA_ANALYSISDATA_H

#include <map>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;
using namespace std;

class AnalysisData {
   public:
    AnalysisData(const int reps, const double &avgFitness,
                 const double &avgTime, const std::vector<double> &bestsFitness,
                 const map<int, float> &avgEvolution);

    virtual ~AnalysisData();

    int getRepetitions() const { return repetitions; }

    void setRepetitions(int repetitions) {
        AnalysisData::repetitions = repetitions;
    }

    double getAvgFitness() const { return avgFitness; }

    void setAvgFitness(double avgFitness) {
        AnalysisData::avgFitness = avgFitness;
    }

    double getAverageElapsedTime() const { return averageElapsedTime; }

    void setAverageElapsedTime(double averageElapsedTime) {
        AnalysisData::averageElapsedTime = averageElapsedTime;
    }

    const vector<double> &getBestFitness() const { return bestFitness; }

    void setBestFitness(const vector<double> &bestFitness) {
        AnalysisData::bestFitness = bestFitness;
    }

    const map<int, float> &getAverageEvolution() const {
        return averageEvolution;
    }

    void setAverageEvolution(const map<int, float> &averageEvolution) {
        AnalysisData::averageEvolution = averageEvolution;
    }

    json to_json() const;

   private:
    int repetitions;

    double avgFitness;

    double averageElapsedTime;

    vector<double> bestFitness;

    map<int, float> averageEvolution;

   private:
    const string AVG_OBJ;
    const string AVG_TIME;
    const string REPS;
    const string AVG_EVO;
    const string BEST_OBJS;
};

#endif  // DIGNEA_ANALYSISDATA_H
