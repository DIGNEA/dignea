

#include <dignea/distances/Euclidean.h>
#include <dignea/factories/NSFactory.h>
#include <dignea/mea/solutions/IKPSolution.h>
#include <dignea/types/SolutionTypes.h>

#include <nlohmann/json.hpp>

#include <catch2/catch_all.hpp>

using IS = IKPSolution;
using json = nlohmann::json;

TEST_CASE("NSFactory Tests", "[NSFactory]") {
    SECTION("Creating a Standard Novelty Search") {
        auto thresholdNS = 3;
        auto k = 3;
        auto distance = make_unique<Euclidean<float>>();
        auto novelty = NSFactory<IS>().create(NSType::Standard, move(distance),
                                              thresholdNS, k);
        auto data = novelty->to_json();
        REQUIRE(novelty);
        REQUIRE(data.is_object());
        REQUIRE(data["name"] == "Novelty Search");
        REQUIRE(data["k"] == k);
        REQUIRE(data["threshold"] == thresholdNS);
    }
    SECTION("Creating a Features Novelty Search") {
        auto thresholdNS = 3;
        auto k = 3;
        auto distance = make_unique<Euclidean<float>>();
        auto novelty = NSFactory<IS>().create(NSType::Features, move(distance),
                                              thresholdNS, k);
        auto data = novelty->to_json();
        REQUIRE(novelty);
        REQUIRE(data.is_object());
        REQUIRE(data["name"] == "NSFeatures");
        REQUIRE(data["k"] == k);
        REQUIRE(data["threshold"] == thresholdNS);
    }
    SECTION("Creating a Performance Novelty Search") {
        auto thresholdNS = 3;
        auto k = 3;
        auto distance = make_unique<Euclidean<float>>();
        auto novelty = NSFactory<IS>().create(NSType::Performance,
                                              move(distance), thresholdNS, k);
        auto data = novelty->to_json();
        REQUIRE(novelty);
        REQUIRE(data.is_object());
        REQUIRE(data["name"] == "NSPerformance");
        REQUIRE(data["k"] == k);
        REQUIRE(data["threshold"] == thresholdNS);
    }
}