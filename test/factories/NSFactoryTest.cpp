

#include <dignea/distances/Euclidean.h>
#include <dignea/factories/NSFactory.h>
#include <dignea/generator/instances/KPInstance.h>
#include <dignea/types/SolutionTypes.h>

#include <catch2/catch_all.hpp>
#include <nlohmann/json.hpp>

using IS = KPInstance;
using json = nlohmann::json;

TEST_CASE("NSFactory Tests", "[NSFactory]") {
    SECTION("Creating a Standard Novelty Search") {
        auto thresholdNS = 3;
        auto k = 3;
        auto distance = make_unique<Euclidean<float>>();
        auto novelty =
            NSFactory<IS>().create(NSType::Standard, move(distance),
                                   thresholdNS, thresholdNS, k, false);
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
        auto novelty =
            NSFactory<IS>().create(NSType::Features, move(distance),
                                   thresholdNS, thresholdNS, k, false);
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
        auto novelty =
            NSFactory<IS>().create(NSType::Performance, move(distance),
                                   thresholdNS, thresholdNS, k, false);
        auto data = novelty->to_json();
        REQUIRE(novelty);
        REQUIRE(data.is_object());
        REQUIRE(data["name"] == "NSPerformance");
        REQUIRE(data["k"] == k);
        REQUIRE(data["threshold"] == thresholdNS);
    }
}