
#include <dignea/problems/BPP.h>

using namespace std;

/**
 * @brief Construct a new BPP::BPP object
 *
 */
BPP::BPP()
    : Problem<IntIntSolution>(0, 1, 0), capacity(0), pathToInstance("") {}

/**
 * Generates a BPP problem with all the data given in the arguments
 * @param numberOfVars
 * @param capacity
 * @param weights
 * @param profits
 */
BPP::BPP(const int &numberOfVars, const int &capacity, const vector<int> &items)
    : Problem<IntIntSolution>(numberOfVars, 1, 0),
      items(items),
      capacity(capacity),
      pathToInstance("") {}

/**
 * @brief Construct a new BPP::BPP object from another BPP object.
 *
 * @param bpp
 */
BPP::BPP(const BPP *bpp) : Problem<IntIntSolution>() {
    this->numberOfVars = bpp->numberOfVars;
    this->numberOfObjs = bpp->numberOfObjs;
    this->numberOfCons = bpp->numberOfCons;
    this->capacity = bpp->capacity;
    this->items = bpp->items;
    this->pathToInstance = bpp->pathToInstance;
}

/**
 * @brief Construct a new BPP::BPP object from the data in the file in the path.
 *
 * @param path
 */
BPP::BPP(const string &path) : Problem<IntIntSolution>(0, 1, 0) {
    ifstream inputFile;
    inputFile.exceptions(ifstream::badbit);  // No need to check failbit
    std::string error = "There was an error reading BPP instance.";
    try {
        this->pathToInstance = path;
        inputFile.open(path);
        if (!inputFile.is_open()) {
            error += " File not found";
            throw(std::runtime_error(error));
        }
        inputFile >> this->numberOfVars >> this->capacity;
        this->items.reserve(this->numberOfVars);
        int w;
        while (inputFile >> w) {
            this->items.push_back(w);
        }
        inputFile.close();

    } catch (const exception &e) {
        throw(std::runtime_error(error));
    }
}

/**
 * @brief Reads the data from the file in the path.
 * The file should contain a proper BPP instance with the following format:
 * nItems Q
 * w0
 * w1
 * ...
 * wn-1
 *
 * @param path
 */
void BPP::readData(const string &path) {
    ifstream inputFile;
    inputFile.exceptions(ifstream::badbit);  // No need to check failbit
    std::string error = "There was an error reading BPP instance.";
    try {
        this->pathToInstance = path;
        inputFile.open(path);
        if (!inputFile.is_open()) {
            error += " File not found";
            throw(std::runtime_error(error));
        }
        inputFile >> this->numberOfVars >> this->capacity;
        this->items.reserve(this->numberOfVars);
        int w;
        while (inputFile >> w) {
            this->items.push_back(w);
        }
        inputFile.close();

    } catch (const exception &e) {
        throw(std::runtime_error(error));
    }
}

/**
 * @brief Returns the upper limit of the variable i
 * In this case, the upper limit is the number of items - 1
 *
 * @param i
 * @return float
 */
float BPP::getUpperLimit(int i) const {
    if (i < 0 || i >= this->numberOfVars) {
        std::string where = "getUpperLimit in KNP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return this->numberOfVars - 1;
}

/**
 * @brief Returns the lower limit of the variable i
 * In this case, the lower limit is 0
 *
 * @param i
 * @return float
 */
float BPP::getLowerLimit(int i) const {
    if (i < 0 || i >= this->numberOfVars) {
        std::string where = "getLowerLimit in KNP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return 0;
}

/**
 * @brief Evaluates a bin assignment for the BPP instances represented in the
 * object
 * The fitness of the solution is the amount of unused space, as well as the
 * number of bins for a specific solution. Falkenauer (1998) performance metric
 * defined as:
 *
 * (x) = \frac{\sum_{k=1}^{N} \left(\frac{fill_k}{C}\right)^2}{N}
 *
 * @param solution
 */
void BPP::evaluate(IntIntSolution &solution) const {
    // Vector to track the remaining capacity of each bin
    map<int, int> filled;
    vector assignment = solution.getVariables();
    for (int i = 0; i < this->numberOfVars; i++) {
        auto item = this->items[i];
        auto assignedBin = assignment[i];
        if (filled.find(assignedBin) == filled.end()) {
            filled[assignedBin] = item;
        } else {
            filled[assignedBin] += item;
        }
    }
    // Finds the number of bins used
    float fitness = 0.0f;
    for (auto &[_, fill_i] : filled) {
        float fill_by_capacity = ((float)fill_i / (float)(this->capacity));
        fitness += (fill_by_capacity) * (fill_by_capacity);
    }

    auto usedBins = *std::max_element(assignment.begin(), assignment.end()) + 1;
    fitness /= usedBins;
    // Defines the fitness as the number of bins used to store the items
    solution.setFitness(fitness);
    solution.setObjAt(0, usedBins);
}

/**
 * @brief Creates a random solution for the BPP
 * Each items is assigned to a random bin in the range [0, numberOfVars)
 *
 * @return IntIntSolution
 */
IntIntSolution BPP::createSolution() const {
    IntIntSolution solution(this->numberOfVars, 1, 0);
    // Creates a random assignment for each item
    vector bins(this->numberOfVars, 0);
    std::iota(begin(bins), end(bins), 0);
    std::random_shuffle(begin(bins), end(bins));
    solution.setVariables(bins);
    return solution;
}

/**
 * @brief Creates a random solution for the BPP
 * Each items is assigned to a random bin in the range [0, numberOfVars)
 *
 * @return IntIntSolution
 */
IntIntSolution BPP::createSolution(ParallelPRNG &engine) const {
    return this->createSolution();
}

/**
 * @brief Updates the capacity of the instance
 *
 * @param Q
 */
void BPP::setCapacity(int Q) { capacity = Q; }

/**
 * @brief Gets the optimization direction of the problem. For the BPP is should
 * always be Minimize
 *
 * @param i
 * @return int
 */
int BPP::getOptimizationDirection(const int i) const {
    if (i < 0 || i >= this->getNumberOfObjs()) {
        std::string where =
            "getOptimizationDirection in KNP with index = " + to_string(i);
        throw(OutOfRange(where));
    }
    return Minimize;
}

/**
 * Generates a JSON object with the information of the BPP problem
 * @return
 */
json BPP::to_json() const {
    json data = Problem<IntIntSolution>::to_json();
    data["capacity"] = this->capacity;
    data["items"] = this->items;

    return data;
}
