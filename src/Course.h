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
    Course(string Code, int Loc, int start, int end)
    {
        classCode = Code;
        LocID = Loc;
        startTime = start;
        endTime = end;
    }
//Getters
    string GetClassCode(){return classCode;}
    int GetLocID(){return LocID;}
    int GetTimeBuffer(Course& rhs){return (rhs.startTime - this->endTime);}
    vector<int> GetRoster(){return roster;}
//Methods
    bool removeFromRoster(int studentID)
    {
        for(auto it = roster.begin(); it != roster.end(); it++)
        {
            if(*it == studentID)
            {
                roster.erase(it);
                return true;
            }
        }
        return false;
    }

    bool addToRoster(int studentID)
    {
        for(auto it = roster.begin(); it != roster.end(); it++)
        {
            if(*it == studentID)
            {
                return false;
            }
        }
        roster.push_back(studentID);
        return true;
    }

    bool swapCourse(int studentID, Course& other)
    {
        if(!removeFromRoster(studentID)){return false;};
        if(!other.addToRoster(studentID)){return false;};
        return true;
    }

    bool closeCourse()
    {
        if(roster.size() < 1){return false;}
        roster.clear();
        return true;
    }
};