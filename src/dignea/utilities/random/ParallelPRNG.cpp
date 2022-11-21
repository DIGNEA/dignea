//
// Created by amarrero on 5/2/21.
//

#include <dignea/utilities/random/ParallelPRNG.h>
//
// Created by amarrero on 15/12/20.
//

ParallelPRNG::ParallelPRNG() {
    randomGenerator_ = std::make_unique<RandomGenerator>();
}

ParallelPRNG::~ParallelPRNG() {}

double ParallelPRNG::randDouble() {
    return randomGenerator_->rndreal(0.0, 1.0);
}

int ParallelPRNG::randInt(int minBound, int maxBound) {
    return randomGenerator_->rnd(minBound, maxBound);
}

double ParallelPRNG::randDouble(double minBound, double maxBound) {
    return randomGenerator_->rndreal(minBound, maxBound);
}

/**
 * Use the polar form of the Box-Muller transformation to obtain
 * a pseudo random number from a Gaussian distribution
 * Code taken from Maurice Clerc's implementation
 * @param mean
 * @param standardDeviation
 * @return A pseudo random number
 */
double ParallelPRNG::randNormal(double mean, double standardDeviation) {
    double x1, x2, w, y1;

    do {
        x1 = 2.0 * randDouble() - 1.0;
        x2 = 2.0 * randDouble() - 1.0;
        w = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;
    y1 = y1 * standardDeviation + mean;
    return y1;
}

/**
 * Get a random point from an hypersphere (center = 0, radius = 1)
 * Code taken from Maurice Clerc's implementation
 * @param dimension
 * @return A pseudo random point
 */
double *ParallelPRNG::randSphere(int dimension) {
    int D = dimension;
    double *x = new double[dimension];

    double length = 0;
    for (int i = 0; i < dimension; i++) x[i] = 0.0;

    // --------- Step 1. Direction

    for (int i = 0; i < D; i++) {
        x[i] = randNormal(0, 1);
        length += length + x[i] * x[i];
    }

    length = sqrt(length);

    // --------- Step 2. Random radius

    double r = randDouble(0, 1);

    for (int i = 0; i < D; i++) {
        x[i] = r * x[i] / length;
    }

    return x;
}
