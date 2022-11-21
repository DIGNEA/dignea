

/**
 * @file TSP.cpp
 * @author Alejandro Marrero
 * @brief TSP class implementation file
 * @version 0.1
 * @date 2022-03-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <dignea/problems/TSP.h>

#include <algorithm>
#include <random>
#include <set>

using namespace std;

/**
 * @brief Construct a new TSP::TSP object
 *
 */
TSP::TSP()
    : Problem<IntFloatSolution>(0, 1, 1),
      pathToInstance(""),
      points(),
      distances() {}

/**
 * @brief Construct a new TSP::TSP object
 *
 * @param numberOfVars
 */
TSP::TSP(const int &numberOfVars)
    : Problem(numberOfVars, 1, 1), pathToInstance(""), points(), distances() {}

/**
 * @brief Construct a new TSP::TSP object
 *
 * @param numberOfVars
 * @param coordinates
 */
TSP::TSP(const int &numberOfVars, const vector<coords> &coordinates)
    : Problem(numberOfVars, 1, 1), pathToInstance(""), points(coordinates) {
    computeDistances();
}

/**
 * Crea una instancia del TSP a partir de la instancia en el
 * directorio recibido
 * @param pathTo
 */
TSP::TSP(const string &path)
    : Problem<IntFloatSolution>(0, 1, 1),
      pathToInstance(path),
      points(),
      distances() {
    ifstream inputFile;
    inputFile.exceptions(ifstream::badbit);  // No need to check failbit
    std::string error = "There was an error reading TSP instance.";
    try {
        inputFile.open(path);
        if (!inputFile.is_open()) {
            error += " File not found";
            throw(std::runtime_error(error));
        }
        inputFile >> this->numberOfVars;
        this->points.reserve(numberOfVars);
        float x, y;
        while (inputFile >> x >> y) {
            points.push_back(std::make_pair(x, y));
        }

        inputFile.close();
    } catch (const exception &e) {
        throw(std::runtime_error(error));
    }
    computeDistances();
}

/**
 * @brief Construct a new TSP::TSP object
 *
 * @param tsp
 */
TSP::TSP(TSP const *tsp) : Problem<IntFloatSolution>() {
    this->numberOfVars = tsp->numberOfVars;
    this->numberOfObjs = tsp->numberOfObjs;
    this->numberOfCons = tsp->numberOfCons;
    this->pathToInstance = tsp->pathToInstance;
    this->points = tsp->points;
    this->distances = tsp->distances;
}

/**
 * Método para leer un fichero de instancia del KP.
 * La estructura esperada del fichero es la siguiente:
 * nItems Q
 * w0 p0
 * w1 p1
 * ...
 * wn-1 pn-1
 * @param path
 */
void TSP::readData(const string &path) {
    ifstream inputFile;
    inputFile.exceptions(ifstream::badbit);  // No need to check failbit
    std::string error = "There was an error reading TSP instance.";
    try {
        this->pathToInstance = path;
        inputFile.open(path);
        if (!inputFile.is_open()) {
            error += " File not found";
            throw(std::runtime_error(error));
        }
        inputFile >> this->numberOfVars;
        this->points.reserve(numberOfVars);
        float x, y;
        while (inputFile >> x >> y) {
            points.push_back(std::make_pair(x, y));
        }
        inputFile.close();
    } catch (const exception &e) {
        throw(std::runtime_error(error));
    }
    computeDistances();
}

/**
 * @brief Gets upper limit for TSP which it's the amount of available cities in
 * the instance
 *
 * @param i
 * @return float
 */
float TSP::getUpperLimit(int i) const {
    if (i < 0 || i >= this->numberOfVars) {
        std::string where = "getUpperLimit in TSP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return this->numberOfVars - 1;
}

/**
 * @brief Gets lower limit for TSP. In this case it's fixed to 1.
 *
 * @param i
 * @return float
 */
float TSP::getLowerLimit(int i) const {
    if (i < 0 || i >= this->numberOfVars) {
        std::string where = "getLowerLimit in TSP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return 1;
}

/**
 * @brief Evaluates whether the solution does not contain a point more than once
 * Also checks that the solution starts and ends in the first city (aka point 0)
 *
 * @param solution
 * @return true
 * @return false
 */
bool TSP::evaluateConstraints(IntFloatSolution &solution) const {
    std::set<int> inSolution;
    vector<int> vars = solution.getVariables();
    const unsigned int size = vars.size();
    if (vars[0] != 0 || vars[vars.size() - 1] != 0) {
        return false;
    }
    inSolution.insert(0);
    for (unsigned int i = 1; i < size - 1; i++) {
        if (inSolution.count(vars[i]) != 0) {
            return false;
        } else {
            inSolution.insert(vars[i]);
        }
    }

    return true;
}

/**
 * Metodo de evaluacion de una solucion al problema KP
 * @param solution
 */
void TSP::evaluate(IntFloatSolution &solution) const {
    if (this->distances.empty()) {
        throw runtime_error(
            "Cannot evaluate solution in TSP since distance vector is empty.");
    }
    if (solution.getNumberOfVars() != this->getNumberOfVars() + 1) {
        throw runtime_error(
            "Cannot evaluate solution in TSP since dimensions are different. "
            "Dimension is: " +
            to_string(this->getNumberOfVars() + 1) +
            " and the individual has dimension: " +
            to_string(solution.getNumberOfVars()));
    }
    float distance = 0.0;
    float penalty = 0.0;
    if (evaluateConstraints(solution)) {
        penalty = 0;
        vector<int> variables = solution.getVariables();
        const unsigned int size = variables.size();
        for (unsigned int i = 0; i < size - 1; i++) {
            distance += this->distances[variables[i]][variables[i + 1]];
        }

    } else {
        penalty = std::numeric_limits<float>::max();
        distance = std::numeric_limits<float>::max();
    }
    float fitness = 1.0 / distance;
    solution.setFitness(fitness);
    solution.setObjAt(0, distance);
    solution.setConstraintCoeff(penalty);
    solution.setConstraints({penalty});
}

/**
 * @brief Creates a new solution for the TSP
 *
 * @return IntFloatSolution
 */
IntFloatSolution TSP::createSolution() const {
    IntFloatSolution solution(this->numberOfVars + 1, 1, 1);
    vector<int> vars(this->numberOfVars + 1);  // For extra the zero at the end
    std::iota(std::begin(vars), std::end(vars) - 1, 0);
    vars[vars.size() - 1] = 0;
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
    std::shuffle(std::begin(vars) + 1, std::end(vars) - 1, rng);
    solution.setVariables(vars);
    return solution;
}

int TSP::getOptimizationDirection(const int i) const {
    if (i < 0 || i >= this->getNumberOfObjs()) {
        std::string where =
            "getOptimizationDirection in TSP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return Minimize;
}

/**
 * @brief Computes the distance from each point to the other in the instance
 *
 */
void TSP::computeDistances() {
    distances.resize(this->numberOfVars,
                     vector<float>(this->numberOfVars, 0.0f));
    for (int i = 0; i < this->numberOfVars; i++) {
        for (int j = i + 1; j < this->numberOfVars; j++) {
            distances[i][j] = ((points[j].first - points[i].first) *
                               (points[j].first - points[i].first)) +
                              ((points[j].second - points[i].second) *
                               (points[j].second - points[i].second));
            distances[j][i] = distances[i][j];
        }
    }
}

/**
 * @brief Creates a solution for the TSP using a Parallel RNG
 *
 * @param engine
 * @return IntFloatSolution
 */
IntFloatSolution TSP::createSolution(ParallelPRNG &engine) const {
    throw runtime_error("Cannot create TSP instance using PRNG ");
}

/**
 * Generates a JSON object with the information of the KP problem
 * @return
 */
json TSP::to_json() const {
    json data = Problem<IntFloatSolution>::to_json();
    data["points"] = this->points;
    data["distances"] = this->distances;
    return data;
}
