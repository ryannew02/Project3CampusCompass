#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "CampusCompass.h"

using namespace std;


static CampusCompass& ParsedBase() {
    static CampusCompass testBase = [](){
        CampusCompass graph;
        if(!graph.ParseCSV("data/edges.csv", "data/classes.csv")) {
            throw std::runtime_error("Failed to parse CSV fixture data");
        }
        return graph;
    }();
    return testBase;
}

TEST_CASE("Testing Incorrect insert Commands", "[ParseCommand][insert]")
{
    CampusCompass testGraph = ParsedBase();

    SECTION("invalid name (contains digits)") {
        REQUIRE_FALSE(testGraph.ParseCommand("insert \"Ry4n\" 45679999 1 1 COP3530"));
    }
    SECTION("invalid UFID (7 digits, not 8)") {
        REQUIRE_FALSE(testGraph.ParseCommand("insert \"Ryan\" 4567999 1 1 COP3530"));
    }
    SECTION("invalid residence ID (not a valid location)") {
        REQUIRE_FALSE(testGraph.ParseCommand("insert \"Courtney\" 45679999 999999 1 COP3530"));
    }
    SECTION("N doesn't match number of class codes given") {
        REQUIRE_FALSE(testGraph.ParseCommand("insert \"Ryan\" 45679999 1 2 COP3530"));
    }
    SECTION("nonexistent course") {
        REQUIRE_FALSE(testGraph.ParseCommand("insert \"Kevin\" 45679999 1 1 FAKE9999"));
    }
    SECTION("unrecognized command") {
        REQUIRE_FALSE(testGraph.ParseCommand("inser \"Mitchell\" 45679999 1 1 COP3530"));
    }
}

TEST_CASE("Testing Incorrect remove Commands", "[ParseCommand][remove]")
{
    CampusCompass testGraph = ParsedBase();

    SECTION("student doesn't exist") {
        REQUIRE_FALSE(testGraph.ParseCommand("remove 45679999"));
    }
    SECTION("invalid UFID (7 digits)") {
        REQUIRE_FALSE(testGraph.ParseCommand("remove 4567999"));
    }
    SECTION("removing correctly") {
        REQUIRE(testGraph.ParseCommand("insert \"Sarah\" 45679999 1 1 COP3530"));
        REQUIRE(testGraph.ParseCommand("remove 45679999"));
    }
}

TEST_CASE("Testing Incorrect dropClass Commands", "[ParseCommand][dropClass]")
{
    CampusCompass testGraph = ParsedBase();
    REQUIRE(testGraph.ParseCommand("insert \"Andrew\" 45679999 1 1 COP3530"));

    SECTION("student not enrolled in that class") {
        REQUIRE_FALSE(testGraph.ParseCommand("dropClass 45679999 CDA3101"));
    }
    SECTION("student doesn't exist") {
        REQUIRE_FALSE(testGraph.ParseCommand("dropClass 11111111 COP3530"));
    }
    SECTION("dropping correctly") {
      REQUIRE(testGraph.ParseCommand("dropClass 45679999 COP3530"));
    }
}

TEST_CASE("Testing Incorrect replaceClass Commands", "[ParseCommand][replaceClass]")
{
    CampusCompass testGraph = ParsedBase();
    REQUIRE(testGraph.ParseCommand("insert \"David\" 45679999 1 2 COP3530 MAC2311"));

    SECTION("not enrolled in CLASSCODE_A") {
        REQUIRE_FALSE(testGraph.ParseCommand("replaceClass 45679999 CDA3101 EEL3701"));
    }
    SECTION("already enrolled in CLASSCODE_B") {
        REQUIRE_FALSE(testGraph.ParseCommand("replaceClass 45679999 COP3530 MAC2311"));
    }
    SECTION("correct functionality") {
        REQUIRE(testGraph.ParseCommand("replaceClass 45679999 COP3530 EEL3701"));
    }
}

TEST_CASE("Testing Incorrect removeClass Commands", "[ParseCommand][removeClass]")
{
    CampusCompass testGraph = ParsedBase();

    SECTION("class code doesn't exist") {
        REQUIRE_FALSE(testGraph.ParseCommand("removeClass FAKE9999"));
    }
    SECTION("class exists but has 0 students enrolled") {
        REQUIRE_FALSE(testGraph.ParseCommand("removeClass COP3530"));
    }
    SECTION("removing correctly") {
        REQUIRE(testGraph.ParseCommand("insert \"Alex\" 45679999 1 1 COP3530"));
        REQUIRE(testGraph.ParseCommand("removeClass COP3530"));
    }
}

TEST_CASE("printShortestEdges", "[printShortestEdges][graph]")
{
    CampusCompass testGraph = ParsedBase();

    REQUIRE(testGraph.ParseCommand("insert \"Ryan Juergens\" 11112222 20 1 COP3502"));

    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    testGraph.ParseCommand("printShortestEdges 11112222");
    std::string before = buffer.str();

    buffer.str("");
    testGraph.ParseCommand("toggleEdgesClosure 1 8 20");
    testGraph.ParseCommand("printShortestEdges 11112222");
    std::string after = buffer.str();

    std::cout.rdbuf(oldCout);

    REQUIRE(before.find("COP3502: -1") == std::string::npos);
    REQUIRE(after.find("COP3502: -1") != std::string::npos);
}
