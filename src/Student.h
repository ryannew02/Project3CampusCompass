#pragma once
#include <string>

using namespace std;

class Student{
private:
    string name;
    int UFID;
    int residenceID;
    vector<string> courseSchedule;
    
public:
//Constructor
    Student(string studentName, int studentID, int resID, vector<string> courseCodes)
{
    name = studentName;
    UFID = studentID;
    residenceID = resID;
    for(auto i : courseCodes)
    {
        courseSchedule.push_back(i);
    }
}
//Getters
    string GetName(){return name;}
    int GetUFID(){return UFID;}
    int GetResidenceID(){return residenceID;}
    vector<string> GetCourseSchedule(){return courseSchedule;}
//Methods
    bool addCourse(string courseCode)
    {
        for(auto it = courseSchedule.begin(); it != courseSchedule.end(); it++)
        {
            if(*it == courseCode)
            {
                return false;
            }
        }
        courseSchedule.push_back(courseCode);
        return true;
    }

    bool removeCourse(string courseCode)
    {
        for(auto it = courseSchedule.begin(); it != courseSchedule.end(); it++)
        {
            if(*it == courseCode)
            {
                courseSchedule.erase(it);
                return true;
            }
        }
        return false;
    }

    bool swapCourse(string oldCourseCode, string newCourseCode)
    {
        if(!removeCourse(oldCourseCode)){return false;};
        if(!addCourse(newCourseCode)){return false;};
        return true;
    }

    bool removeStudent()
    {
        courseSchedule.clear();
        return true;
    }
};