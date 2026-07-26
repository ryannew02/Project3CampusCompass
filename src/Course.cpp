#include <Course.h>

using namespace std;

    Course::Course(string Code, int Loc, int start, int end)
    {
        classCode = Code;
        LocID = Loc;
        startTime = start;
        endTime = end;
    }
//Getters
    string Course::GetClassCode(){return classCode;}
    int Course::GetLocID(){return LocID;}
    int Course::GetTimeBuffer(Course& rhs){return (rhs.startTime - this->endTime);}
    vector<int> Course::GetRoster(){return roster;}
//Methods
    bool Course::removeFromRoster(int studentID)
    {
        for(auto it = roster.begin(); it != roster.end();)
        {
            if(*it == studentID)
            {
                roster.erase(it);
                return true;
            }
            else{it++;}
        }
        return false;
    }

    bool Course::addToRoster(int studentID)
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

    bool Course::swapCourse(int studentID, Course& other)
    {
        if(!removeFromRoster(studentID)){return false;};
        if(!other.addToRoster(studentID)){return false;};
        return true;
    }

    bool Course::closeCourse()
    {
        if(roster.size() < 1){return false;}
        roster.clear();
        return true;
    }

    bool Course::isEnrolledIn(int studentID)
    {
        for(auto it = roster.begin(); it != roster.end();)
        {
            if(*it == studentID)
            {
                return true;
            }
            else{it++;}
        }
        return false;
    }