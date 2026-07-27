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
        REQUIRE_FALSE(testGraph.ParseCommand("insert \"A11y\" 45679999 1 1 COP3530"));
    }
    SECTION("invalid UFID (7 digits, not 8)") {
        REQUIRE_FALSE(testGraph.ParseCommand("insert \"Alex\" 4567999 1 1 COP3530"));
    }
    SECTION("invalid residence ID (not a valid location)") {
        REQUIRE_FALSE(testGraph.ParseCommand("insert \"Alex\" 45679999 999999 1 COP3530"));
    }
    SECTION("N doesn't match number of class codes given") {
        REQUIRE_FALSE(testGraph.ParseCommand("insert \"Alex\" 45679999 1 2 COP3530"));
    }
    SECTION("nonexistent course") {
        REQUIRE_FALSE(testGraph.ParseCommand("insert \"Alex\" 45679999 1 1 FAKE9999"));
    }
    SECTION("unrecognized command") {
        REQUIRE_FALSE(testGraph.ParseCommand("inser \"Alex\" 45679999 1 1 COP3530"));
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
}

TEST_CASE("Testing Incorrect dropClass Commands", "[ParseCommand][dropClass]")
{
    CampusCompass testGraph = ParsedBase();
    REQUIRE(testGraph.ParseCommand("insert \"Alex\" 45679999 1 1 COP3530"));

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
    REQUIRE(testGraph.ParseCommand("insert \"Alex\" 45679999 1 2 COP3530 MAC2311"));

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

//==========Need to add more ParseCommand incorrect commands====================

//==========Need to add more testing for printShortestEdges command in a case where a student can reach a class, then one or more edges turn off, and then they cannot reach that class. (Hint: the visualizer tool can help with this a lot). [2 points] ==============



// Refer to Canvas for a list of required tests. 
// We encourage you to write more than required to ensure proper functionality, but only the ones on Canvas will be graded.

// See the following for an example of how to easily test your output.
// Note that while this works, I recommend also creating plenty of unit tests for particular functions within your code.
// This pattern should only be used for final, end-to-end testing.

// This uses C++ "raw strings" and assumes your CampusCompass outputs a string with
//   the same thing you print.
// TEST_CASE("Example CampusCompass Output Test", "[flag]") {
//   // the following is a "raw string" - you can write the exact input (without
//   //   any indentation!) and it should work as expected
//   // this is based on the input and output of the first public test case
//   string input = R"(6
// insert "Student A" 10000001 1 1 COP3502
// insert "Student B" 10000002 1 1 COP3502
// insert "Student C" 10000003 1 2 COP3502 MAC2311
// dropClass 10000001 COP3502
// remove 10000001
// removeClass COP3502
// )";

//   string expectedOutput = R"(successful
// successful
// successful
// successful
// unsuccessful
// 2
// )";

//   string actualOutput;

//   // somehow pass your input into your CampusCompass and parse it to call the
//   // correct functions, for example:
//   /*
//   CampusCompass c;
//   c.parseInput(input)
//   // this would be some function that sends the output from your class into a string for use in testing
//   actualOutput = c.getStringRepresentation()
//   */

//   REQUIRE(actualOutput == expectedOutput);
// }
