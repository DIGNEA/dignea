#include <dignea/utilities/printer/JSONPrinter.h>

JSONPrinter::JSONPrinter(string filename)
    : outputFPattern(filename), extension(".json"), dataToPrint() {}

/**
 * @brief Prints all data collected into a file and clears the status
 *
 * @return true
 * @return false
 */
bool JSONPrinter::flush() {
    if (this->dataToPrint.empty()) {
        std::cerr << "Nothing to flush since dataToPrint is empty";
        return false;
    }
    try {
        ofstream outputFile(outputFPattern + extension);
        outputFile << std::setw(4) << dataToPrint;
        outputFile.close();
        dataToPrint.clear();
        return true;
    } catch (std::exception &e) {
        std::cerr << "Something went wrong: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief Prints the json data in the filename given. This is an auxiliar method
 * to generate separated JSON files or if you don't want to collect a lot of
 * information.
 *
 * @param filename
 * @param data
 */
void JSONPrinter::print(const string &filename, json &data) {
    ofstream outputFile(filename + extension);
    outputFile << std::setw(4) << data;
    outputFile.close();
}

/**
 * @brief Includes the data under the given tag into the dataToPrint
 *
 * @param tag
 * @param data
 */
void JSONPrinter::append(const std::string &tag, json &data) {
    this->dataToPrint[tag] = data;
}