//
// Created by amarrero on 16/12/20.
//

#include "dignea/problems/KP.h"

using namespace std;

const string KP::INSTANCE = "Instance";
const string KP::CAPACITY = "Capacity";
const string KP::WEIGHTS = "Weights";
const string KP::PROFITS = "Profits";

KP::KP() : Problem<BoolFloatSolution>(0, 1, 1) {
    lowWeight = 0;
    upWeight = 0;
    lowProfit = 0;
    upProfit = 0;
    capacity = 0;
    pathToInstance = "";
}

KP::KP(const int &numberOfVars) : Problem(numberOfVars, 1, 1) {
    lowWeight = 0;
    upWeight = 0;
    lowProfit = 0;
    upProfit = 0;
    capacity = 0;
    pathToInstance = "";
    weights.reserve(numberOfVars);
    profits.reserve(numberOfVars);
    int totalWeight = 0;
    for (int i = 0; i < numberOfVars; i++) {
        profits.push_back(PseudoRandom::randInt(1, 100));
        int w = PseudoRandom::randInt(1, 100);
        weights.push_back(w);
        totalWeight += w;
    }
    capacity = (int)(totalWeight * 0.8);
    computeEfficiency();
}

KP::KP(const int &numberOfVars, const int &lowerWeight, const int &upperWeight,
       const int &lowerProfit, const int &upperProfit)
    : Problem(numberOfVars, 1, 1),
      lowWeight(lowerWeight),
      upWeight(upperWeight),
      lowProfit(lowerProfit),
      upProfit(upperProfit),
      pathToInstance() {
    profits.reserve(numberOfVars);
    weights.reserve(numberOfVars);
    capacity = 0;
    int totalWeight = 0;
    for (int i = 0; i < numberOfVars; i++) {
        profits.push_back(PseudoRandom::randInt(lowProfit, upProfit));
        int w = PseudoRandom::randInt(lowWeight, upWeight);
        weights.push_back(w);
        totalWeight += w;
    }
    capacity = (int)(totalWeight * 0.8);
    computeEfficiency();
}

/**
 * Generates a KP problem with all the data given in the arguments
 * @param numberOfVars
 * @param capacity
 * @param weights
 * @param profits
 */
KP::KP(const int &numberOfVars, const int &capacity,
       const vector<int> &newWeights, const vector<int> &newProfits)
    : Problem<BoolFloatSolution>(numberOfVars, 1, 1) {
    this->profits = newProfits;
    this->weights = newWeights;
    this->capacity = capacity;
    this->computeEfficiency();
}

KP::KP(const KP *knp) : Problem<BoolFloatSolution>() {
    this->numberOfVars = knp->numberOfVars;
    this->numberOfObjs = knp->numberOfObjs;
    this->numberOfCons = knp->numberOfCons;
    this->capacity = knp->capacity;
    this->weights = knp->weights;
    this->profits = knp->profits;
    this->lowWeight = knp->lowWeight;
    this->upWeight = knp->upWeight;
    this->lowProfit = knp->lowProfit;
    this->upProfit = knp->upProfit;
    this->pathToInstance = knp->pathToInstance;
    this->itemsEff = knp->itemsEff;
}

/**
 * Crea una instancia del problema de la mochila a partir de la instancia en el
 * directorio recibido
 * @param pathTo
 */
KP::KP(const string &path) : Problem<BoolFloatSolution>(0, 1, 1) {
    ifstream inputFile;
    inputFile.exceptions(ifstream::badbit);  // No need to check failbit
    std::string error = "There was an error reading KP instance.";
    try {
        this->pathToInstance = path;
        inputFile.open(path);
        if (!inputFile.is_open()) {
            error += " File not found";
            throw(std::runtime_error(error));
        }
        inputFile >> this->numberOfVars >> this->capacity;
        this->weights.reserve(numberOfVars);
        this->profits.reserve(numberOfVars);
        int w, p;
        while (inputFile >> w >> p) {
            weights.push_back(w);
            profits.push_back(p);
        }
        inputFile.close();
    } catch (const exception &e) {
        throw(std::runtime_error(error));
    }
    computeEfficiency();
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
void KP::readData(const string &path) {
    ifstream inputFile;
    inputFile.exceptions(ifstream::badbit);  // No need to check failbit
    std::string error = "There was an error reading KP instance.";
    try {
        this->pathToInstance = path;
        inputFile.open(path);
        if (!inputFile.is_open()) {
            error += " File not found";
            throw(std::runtime_error(error));
        }
        inputFile >> this->numberOfVars >> this->capacity;
        this->weights.reserve(numberOfVars);
        this->profits.reserve(numberOfVars);
        int w, p;
        while (inputFile >> w >> p) {
            weights.push_back(w);
            profits.push_back(p);
        }
        inputFile.close();
    } catch (const exception &e) {
        throw(std::runtime_error(error));
    }
    computeEfficiency();
}

KP::~KP() {
    profits.clear();
    weights.clear();
}

float KP::getUpperLimit(int i) const {
    if (i < 0 || i >= this->numberOfVars) {
        std::string where = "getUpperLimit in KNP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return 1;
}

float KP::getLowerLimit(int i) const {
    if (i < 0 || i >= this->numberOfVars) {
        std::string where = "getLowerLimit in KNP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return 0;
}

/**
 * Evaluacion de las restricciones para la solucion
 * @param ptr
 */
bool KP::evaluateConstraints(BoolFloatSolution &solution) const {
    int packed = 0.0;
    // Asumimos que es factible y la factibilidad es cero
    int diff = 0;
    vector<char> vars = solution.getVariables();
    for (int i = 0; i < this->numberOfVars; i++) {
        packed += this->weights[i] * vars[i];
    }
    diff = packed - this->capacity;
    int penalty = 100 * diff;
    if (penalty < 0) {
        penalty = 0;
    }
    solution.setConstAt(0, (float)penalty);
    solution.setConstraintCoeff((float)penalty);
    return (penalty == 0);
}

/**
 * Heuristica de reparacion que se encarga de eliminar los elementos menos
 * eficientes
 *
 */
void KP::repair(BoolFloatSolution &solution) const {
    int index = 0;
    // Mientras nos pasemos ponemos a false todos los elementos index del vector
    // itemEff
    while (evaluateConstraints(solution) == false) {
        solution.setVarAt(itemsEff[index].first, false);
        index++;
    }
    index = this->numberOfVars - 1;
    while (evaluateConstraints(solution) == false && index >= 0) {
        solution.setVarAt(itemsEff[index].first, true);
        index--;
    }
    // Eliminamos el ultimo elemento incluido porque la capacidad ha sido
    // sobrepasada
    if (index != this->numberOfVars - 1) {
        index++;
    }
    solution.setVarAt(itemsEff[index].first, false);
}

/**
 * Metodo de evaluacion de una solucion al problema KP
 * @param solution
 */
void KP::evaluate(BoolFloatSolution &solution) const {
    float fitness = 0;
    this->repair(solution);
    vector<char> variables = solution.getVariables();
    for (int i = 0; i < this->numberOfVars; i++) {
        fitness += variables[i] * profits[i];
    }
    solution.setFitness(fitness);
    solution.setObjAt(0, fitness);
}

/**
 * Crea una solucion aleatoria con true y false
 * @return
 */
BoolFloatSolution KP::createSolution() const {
    BoolFloatSolution solution(this->numberOfVars, 1, 1);
    vector<char> vars(this->numberOfVars, false);
    for (int i = 0; i < this->numberOfVars; i++) {
        if (PseudoRandom::randDouble() > 0.5) {
            vars[i] = true;
        }
    }
    solution.setVariables(vars);
    return solution;
}

int KP::getLowWeight() const { return lowWeight; }

void KP::setLowWeight(int lWeight) { lowWeight = lWeight; }

int KP::getUpWeight() const { return upWeight; }

void KP::setUpWeight(int uWeight) { upWeight = uWeight; }

int KP::getLowProfit() const { return lowProfit; }

void KP::setLowProfit(int lProfit) { lowProfit = lProfit; }

int KP::getUpProfit() const { return upProfit; }

void KP::setUpProfit(int uProfit) { upProfit = uProfit; }

const vector<int> &KP::getProfits() const { return profits; }

void KP::setProfits(const vector<int> &pro) { profits = pro; }

const vector<int> &KP::getWeights() const { return weights; }

void KP::setWeights(const vector<int> &w) { weights = w; }

int KP::getCapacity() const { return capacity; }

void KP::setCapacity(int Q) { capacity = Q; }

int KP::getOptimizationDirection(const int i) const {
    if (i < 0 || i >= this->getNumberOfObjs()) {
        std::string where =
            "getOptimizationDirection in KNP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return Maximize;
}

/**
 * Calcula la eficiencia de cada elemento de la instancia.
 * La eficiencia de un elemento es el ratio p/w del elemento.
 * Los elementos se ordenan descendentemente
 */
void KP::computeEfficiency() {
    itemsEff.resize(this->numberOfVars);
    for (int j = 0; j < this->numberOfVars; j++) {
        itemsEff[j].first = j;
        itemsEff[j].second = (float)profits[j] / weights[j];
    }
    sort(itemsEff.begin(), itemsEff.end(),
         [&](const iEff &firstItem, const iEff &secondItem) {
             return firstItem.second > secondItem.second;
         });
}

BoolFloatSolution KP::createSolution(ParallelPRNG &engine) const {
    BoolFloatSolution solution(this->numberOfVars, 1, 1);
    vector<char> vars(this->numberOfVars, false);
    for (int i = 0; i < this->numberOfVars; i++) {
        if (engine.randDouble() > 0.5) {
            vars[i] = true;
        }
    }
    solution.setVariables(vars);
    return solution;
}

/**
 * Generates a JSON object with the information of the KP problem
 * @return
 */
json KP::to_json() const {
    json data = Problem<BoolFloatSolution>::to_json();
    data["capacity"] = this->capacity;
    data["profits"] = this->profits;
    data["weights"] = this->weights;
    data["low_profit"] = this->lowProfit;
    data["up_profit"] = this->upProfit;
    data["low_weight"] = this->lowWeight;
    data["up_weight"] = this->upWeight;
    return data;
}
