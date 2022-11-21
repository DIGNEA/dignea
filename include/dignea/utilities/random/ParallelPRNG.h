//
// Created by amarrero on 5/2/21.
//

#ifndef DIGNEA_PARALLELPRNG_H
#define DIGNEA_PARALLELPRNG_H

/**
 * ParallelPRNG.h
 *
 * Author: Alejandro Marrero
 * Contact: alejandro.marrero.10@ull.edu.es
 *
 *  02 - 2021
 **/

#include <dignea/utilities/random/RandomGenerator.h>
#include <math.h>

#include <iostream>
#include <memory>

class ParallelPRNG {
    // private:
   public:
    std::unique_ptr<RandomGenerator> randomGenerator_;

    ParallelPRNG();

    virtual ~ParallelPRNG();

   public:
    double randDouble();  //    static int randInt();

    int randInt(int minBound, int maxBound);

    double randDouble(double minBound, double maxBound);

    double randNormal(double mean, double standardDeviation);

    double *randSphere(int dimension);
};

#endif  // DIGNEA_PSEUDORANDOM_H
