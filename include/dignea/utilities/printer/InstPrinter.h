/**
 * @file InstPrinter.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2020-12-22
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef INST_PRNTER_H
#define INST_PRNTER_H

#include <dignea/core/Front.h>
#include <dignea/mea/MEASolution.h>

#include <fstream>
#include <vector>

using namespace std;

/**
 * @brief Instance Printer class to be used alongside with MEA. Generates the
 * instance representation of the instances generated by MEA.
 *
 * @tparam S
 */
template <typename S>
class InstPrinter {
   public:
    InstPrinter() = delete;

    explicit InstPrinter(const string &filename, const string &ext);

    virtual ~InstPrinter() = default;

    void printInstances(const Front<S> &front);

   private:
    string pattern;
    string extension;
};

/**
 * @brief Construct a new InstPrinter object with a filename and a extension to
 * be used when storing the information in the filesystem.
 *
 * @tparam S
 * @param filename
 * @param ext
 */
template <typename S>
InstPrinter<S>::InstPrinter(const string &filename, const string &ext)
    : pattern(filename), extension(ext) {}

/**
 * @brief Method to create the instance files using the instances generated by
 * MEA. This method uses the <<operator of the MEASolution classes. Be sure to
 * defined that method to generate a proper representation.
 * The method generates a file for each instance in the given front of
 * solutions.
 * @param front
 */
template <typename S>
void InstPrinter<S>::printInstances(const Front<S> &front) {
    auto nSols = front.getNumOfSolutions();
    vector<S> sols = front.getSolutions();
    for (int i = 0; i < nSols; i++) {
        ofstream outFile(pattern + '_' + to_string(i) + extension);
        outFile << sols[i];
        outFile.close();
    }
}

#endif  // INST_PRNTER_H
