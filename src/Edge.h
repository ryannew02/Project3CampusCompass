#pragma once
#include <string>
#include <climits>

using namespace std;

class Edge{

private:
    int LocID1 = -1;
    int LocID2 = -1;
    int weight = INT_MAX;
    bool openStatus = true;

public:
//Getters
    int GetLocID1();
    int GetLocID2();
    int GetToLoc(int from);
    int GetWeight();
    bool GetOpenStatus();

//Methods
    void toggleOpenStatus();

//Constructors
    Edge(int to, int from, int distance);

};