/**
 * @file JSONPrinter.h
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2020-12-22
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef DIGNEA_JSONPRINTER_H
#define DIGNEA_JSONPRINTER_H

#include <dignea/experiment/AnalysisData.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>

using json = nlohmann::json;
using namespace std;
/**
 * @brief JSON Printer Class to generate JSON objects from the information of
 * the different classes in dignea. Uses the to_json() methods.
 *
 */
class JSONPrinter {
   public:
    JSONPrinter() = delete;
    /**
     * @brief Construct a new JSONPrinter with the name of the file to be saved
     * in the filesystem. The class works by collecting information using the
     * append method which receives a tag and json data associated to the tag.
     * When all the data is collected you can call the flush() method to save
     * all the information in a file with the given filename and JSON extension.
     *
     * @param filename
     */
    explicit JSONPrinter(string filename);

    virtual ~JSONPrinter() = default;
    /**
     * @brief Get the Output Filename
     *
     * @return std::string
     */
    inline std::string getOutputFilename() const { return outputFPattern; }

    void print(const string &filename, json &data);

    void append(const std::string &tag, json &data);
    /**
     * @brief Get the collected data to be printed into the file
     *
     * @return json
     */
    inline json getDataToPrint() const { return dataToPrint; }

    bool flush();

   private:
    string outputFPattern;
    string extension;
    json dataToPrint;
};

#endif  // DIGNEA_JSONPRINTER_H
