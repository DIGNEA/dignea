#include <dignea/utilities/KNN.h>

/**
 * @brief Computes the KNN algorithm for the individual
 * Returns the k nearest neighbours Ss in a vector
 *
 * @tparam T
 * @tparam S
 * @param individual
 * @param neighbours
 * @param metric
 * @param k
 * @return vector<S>
 */
neighVector KNN::run(const Descriptor &individual,
                     const vector<Descriptor> &neighbours,
                     Distance<float> *metric, const int &k) {
    if ((unsigned int)k > neighbours.size()) {
        throw runtime_error(
            "K is " + to_string(k) +
            " and neighbours are only: " + to_string(neighbours.size()));
    }
    neighVector distances = {};
    distances.reserve(neighbours.size());
    for (unsigned int i = 0; i < neighbours.size(); i++) {
        distances.push_back({metric->compute(individual, neighbours[i]), i});
    }
    sort(distances.begin(), distances.end());
    return {distances.begin(), distances.begin() + k};
}

/**
 * @brief Computes the KNN for the entire vector of individuals given
 *
 * @tparam S
 * @tparam T
 * @param population
 * @param metric
 * @param k
 * @return neighMatrix<T>
 */
neighMatrix KNN::run(const vector<Descriptor> &population,
                     Distance<float> *metric, const int &k) {
    if ((unsigned int)k > population.size()) {
        throw runtime_error(
            "K is " + to_string(k) +
            " and population are only: " + to_string(population.size()));
    }
    neighMatrix nearest;
    nearest.reserve(population.size());
    for (unsigned int i = 0; i < population.size(); i++) {
        neighVector distances = {};
        distances.reserve(population.size() - 1);
        for (unsigned int j = 0; j < population.size(); j++) {
            // For each other individual we compute the distance to i
            if (i == j) continue;
            // vector<T> jVars(tmpJVars.begin(), tmpJVars.end());
            float dist = metric->compute(population[i], population[j]);
            distances.push_back({dist, j});
        }
        // Ordenamos los vecinos de I por su distancia a I
        sort(distances.begin(), distances.end());
        // We only get the K nearest neighbors
        nearest.push_back({distances.begin(), distances.begin() + k});
    }
    return nearest;
}

/**
 * @brief Computes the sparseness of all individuals in the population vector
 * Based on the Novelty Search from Exploiting Open-Endedness to Solve Problems
 * Through the Search for Novelty
 *
 * @tparam T
 * @param individual
 * @param neighbours
 * @param metric
 * @param k
 * @return T
 */
vector<float> sparseness(const vector<Descriptor> &population,
                         Distance<float> *metric, const int k) {
    if ((unsigned int)k > population.size()) {
        throw runtime_error(
            "K is " + to_string(k) +
            " and population are only: " + to_string(population.size()));
    }
    auto knn = make_unique<KNN>();
    neighMatrix nearest = knn->run(population, metric, k);
    vector spars(population.size(), 0.0f);
    float frac = (1.0 / static_cast<float>(k));
    for (unsigned int i = 0; i < population.size(); i++) {
        for (int j = 0; j < k; j++) {
            spars[i] += nearest[i][j].first;
        }
        spars[i] *= frac;
    }

    return spars;
}

/**
 * @brief Computes the sparseness score of the individual against the rest of
 * individuals in the population list
 * Based on the Novelty Search from Exploiting Open-Endedness to Solve Problems
 * Through the Search for Novelty
 *
 * @tparam T
 * @param individual
 * @param neighbours
 * @param metric
 * @param k
 * @return T
 */
float sparseness(const Descriptor &solution,
                 const vector<Descriptor> &population, Distance<float> *metric,
                 const int k) {
    if ((unsigned int)k > population.size()) {
        throw runtime_error(
            "K is " + to_string(k) +
            " and population are only: " + to_string(population.size()));
    }
    auto knn = make_unique<KNN>();
    neighVector nearest = knn->run(solution, population, metric, k);
    float spars = 0.0;
    float frac = (1.0 / static_cast<float>(k));
    for (const neighborT &neighborT : nearest) {
        spars += neighborT.first;
    }
    spars *= frac;
    return spars;
}
