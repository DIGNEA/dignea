
/**
 * @file JSONPrinterTest.cpp
 * @author Alejandro Marrero (amarrerd@ull.edu.es)
 * @brief
 * @version 0.1
 * @date 2022-05-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <dignea/experiment/AnalysisData.h>
#include <dignea/utilities/printer/JSONPrinter.h>

#include <filesystem>
#include <string>

#include <catch2/catch_all.hpp>
#include "nlohmann/json.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

TEST_CASE("JSONPrinter tests", "[JSONPrinter]") {
    std::cout.setstate(std::ios_base::failbit);
    std::cerr.setstate(std::ios_base::failbit);

    SECTION("Creating a JSONPrinter") {
        std::string filename{"my_test_filename"};
        auto printer{JSONPrinter(filename)};
        REQUIRE(printer.getOutputFilename() == filename);
    }

    SECTION("Printing a sample json object") {
        std::string filename{"my_test_filename"};
        auto printer{JSONPrinter(filename)};
        REQUIRE(printer.getOutputFilename() == filename);
        json data;
        data["name"] = "my_sample_name";
        data["runs"] = 10;
        fs::path output{filename + ".json"};
        REQUIRE_NOTHROW(printer.print(filename, data));
        REQUIRE(fs::exists(output));
        REQUIRE(fs::remove(output));
    }

    SECTION("Printing an AnalysisData object") {
        auto reps = 10;
        auto avgFitness = 100.0;
        auto avgTime = 30.0;
        vector<double> bests = {100.0, 100.0, 100.0, 100.0, 100.0,
                                100.0, 100.0, 100.0, 100.0, 100.0};
        map<int, float> avgEvo = {};

        auto data =
            AnalysisData(reps, avgFitness, avgTime, bests, avgEvo).to_json();
        std::string filename{"my_test_filename"};
        auto printer{JSONPrinter(filename)};

        REQUIRE_NOTHROW(printer.print("my_sample_experiment_avg", data));
        fs::path output{"my_sample_experiment_avg.json"};
        REQUIRE(fs::exists(output));
        REQUIRE(fs::remove(output));
    }

    SECTION("Printing an empty data") {
        std::string filename{"my_test_filename"};
        auto printer{JSONPrinter(filename)};
        REQUIRE_FALSE(printer.flush());
    }

    SECTION("Getting empty data") {
        std::string filename{"my_test_filename"};
        auto printer{JSONPrinter(filename)};
        json emptyJSON = printer.getDataToPrint();
        REQUIRE(emptyJSON.empty());
    }

    SECTION("Printing a sample json object built by steps") {
        std::string filename{"my_test_filename"};
        auto printer{JSONPrinter(filename)};
        vector runsData{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

        json personalData;
        personalData["name"] = "my_sample_name";
        personalData["age"] = 30;

        json experimentData;
        experimentData["exp_name"] = "my_first_experiment";
        experimentData["runs"] = runsData;

        printer.append("user", personalData);
        printer.append("experiment", experimentData);

        json returnedJSON = printer.getDataToPrint();
        REQUIRE(returnedJSON["user"]["name"] == "my_sample_name");
        REQUIRE(returnedJSON["user"]["age"] == 30);
        REQUIRE(returnedJSON["experiment"]["exp_name"] ==
                "my_first_experiment");
        REQUIRE(returnedJSON["experiment"]["runs"] == runsData);

        REQUIRE(printer.flush());
        fs::path output{filename + ".json"};
        REQUIRE(fs::exists(output));
        REQUIRE(fs::remove(output));
    }
}