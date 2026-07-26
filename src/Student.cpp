#include <Student.h>

using namespace std;

    Student::Student(string studentName, int studentID, int resID, vector<string> courseCodes)
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
    string Student::GetName(){return name;}
    int Student::GetUFID(){return UFID;}
    int Student::GetResidenceID(){return residenceID;}
    vector<string> Student::GetCourseSchedule(){return courseSchedule;}
//Methods
    bool Student::addCourse(string courseCode)
    {
        for(auto it = courseSchedule.begin(); it != courseSchedule.end();)
        {
            if(*it == courseCode)
            {
                return false;
            }
            else{it++;}
        }
        courseSchedule.push_back(courseCode);
        return true;
    }

    bool Student::removeCourse(string courseCode)
    {
        for(auto it = courseSchedule.begin(); it != courseSchedule.end();)
        {
            if(*it == courseCode)
            {
                courseSchedule.erase(it);
                return true;
            }
            else{it++;}
        }
        return false;
    }

    bool Student::swapCourse(string oldCourseCode, string newCourseCode)
    {
        if(!removeCourse(oldCourseCode)){return false;};
        if(!addCourse(newCourseCode)){return false;};
        return true;
    }

    bool Student::removeStudent()
    {
        courseSchedule.clear();
        return true;
    }

    bool Student::isEnrolledIn(string courseCode)
    {
        for(auto it = courseSchedule.begin(); it != courseSchedule.end();)
        {
            if(*it == courseCode)
            {
                return true;
            }
            else{it++;}
        }
        return false;
    }