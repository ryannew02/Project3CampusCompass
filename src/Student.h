#pragma once
#include <string>
#include <vector>

using namespace std;

class Student{
private:
    string name;
    int UFID;
    int residenceID;
    vector<string> courseSchedule;
    
public:
//Constructor
    Student(string studentName, int studentID, int resID, vector<string> courseCodes);
//Getters
    string GetName();
    int GetUFID();
    int GetResidenceID();
    vector<string> GetCourseSchedule();
//Methods
    bool addCourse(string courseCode);

    bool removeCourse(string courseCode);

    bool swapCourse(string oldCourseCode, string newCourseCode);

    bool removeStudent();
};