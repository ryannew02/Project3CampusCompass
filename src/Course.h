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
    vector<Student> roster;

public:
//Getters
    string GetClassCode(){return classCode;}
    int GetLocID(){return LocID;}
    int GetTimeBuffer(Course& rhs){return (rhs.startTime - this->endTime);}
    vector<Student> GetRoster(){return roster;}
//Methods
    bool removeFromRoster(int studentID)
    {
        for(auto it = roster.begin(); it != roster.end(); it++)
        {
            if(it->GetUFID() == studentID)
            {
                roster.erase(it);
                return true;
            }
        }
        return false;
    }


};