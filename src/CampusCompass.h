#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <Student.h>
#include <Course.h>
#include <Edge.h>

using namespace std;

class CampusCompass {
private:
    vector<vector<pair<int, Edge>>> adjacencyList;
    vector<pair<int, Edge>> edgeList;
    vector<Student> students;
    vector<Course> catalog;

    // Think about what member variables you need to initialize
    // perhaps some graph representation?

public:    
    //Constructor
    CampusCompass();

    //Data Manipulators
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);

    //Getters
    vector<vector<pair<int, Edge>>> GetAdjacencyList();
    vector<pair<int, Edge>> GetEdgeList();
    vector<Student> GetStudents();
    vector<Course> GetCatalog();

    //Modifiers
    bool addNewStudent(string studentName, int studentID, int residenceID, vector<string> courseCodes);
    bool removeStudent(int studentID);
    bool dropStudent(int studentID, string courseCode);
    bool swapStudent(int studentID, string courseCodeA, string courseCodeB);
    bool removeFromCatalog(string courseCode);
    bool closeEdges(vector<pair<int, int>> LocationIDs);
    
    //Reporting
    bool checkEdge(pair<int, int> LocationIDs);
    bool checkRoute(pair<int, int> LocationIDs);
    vector<pair<string, int>> displayAvailableRoutes(int studentID);
    int dispalyStudentZone(int studentID);
    int verifySchedule(int studentID);
};

