#pragma once
#include <string>

using namespace std;

class Edge{

private:
    int LocID1 = -1;
    int LocID2 = -1;
    int weight = INT_MAX;
    bool openStatus = true;

public:
//Getters
    int GetLocID1(){return LocID1;}
    int GetLocID2(){return LocID2;}
    int GetWeight(){return weight;}
    bool GetOpenStatus(){return openStatus;}

//Methods
    void toggleOpenStatus(){openStatus = !openStatus;}

//Constructors
    Edge(int to, int from, int distance)
    {
        LocID1 = to;
        LocID2 = from;
        weight = distance;
        openStatus = true; 
    }

};