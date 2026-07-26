#pragma once
#include <string>
#include <vector>
#include <Student.h>

using namespace std;

class Course{

private:
    string classCode;
    int LocID;
    int startTime;
    int endTime;
    vector<int> roster;

public:
//Constructor
    Course() : classCode(""), LocID(-1), startTime(-1), endTime(-1) {}
    Course(string Code, int Loc, int start, int end);
//Getters
    string GetClassCode();
    int GetLocID();
    int GetTimeBuffer(Course& rhs);
    vector<int> GetRoster();
//Methods
    bool removeFromRoster(int studentID);
    bool addToRoster(int studentID);
    bool swapCourse(int studentID, Course& other);
    bool closeCourse();
    bool isEnrolledIn(int studentID);
};