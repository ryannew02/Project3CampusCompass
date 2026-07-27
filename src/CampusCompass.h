#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <regex>
#include "Student.h"
#include "Course.h"
#include "Edge.h"

using namespace std;

class CampusCompass {
private:
    map<int, vector<int>> adjacencyList;
    vector<Edge> edgeList;
    map<int, Student> students;
    map<string, Course> catalog;
    set<int> validResidences;
    map<string, int> commandsPossible;
    // Think about what member variables you need to initialize
    // perhaps some graph representation?

public:    
    //Constructor
    CampusCompass();

    //Data Manipulators
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);

    //Getters
    map<int, vector<int>> GetAdjacencyList();
    vector<Edge> GetEdgeList();
    map<int, Student> GetStudents();
    map<string, Course> GetCatalog();

    //Modifiers
    bool removeStudent(int studentID);
    bool dropStudent(int studentID, string courseCode);
    bool swapStudent(int studentID, string courseCodeA, string courseCodeB);
    bool closeEdges(vector<pair<int, int>> LocationIDs);
    
    //Reporting
    bool checkEdge(pair<int, int> LocationIDs);
    bool checkRoute(pair<int, int> LocationIDs);
    vector<pair<string, int>> displayAvailableRoutes(int studentID);
    int dispalyStudentZone(int studentID);
    int verifySchedule(int studentID);

    //Helper
    bool validateName(string name);
    bool validateUFID(string UFID);
    bool validateResidenceID(string residenceID);
    vector<string> splitCommand(string line);
    map<int, pair<int, int>> findDijkstraTable(int source);
    int find(map<int,int>& parent, int x);
    bool isNumber(string param);


};

