#include <Edge.h>

using namespace std;

//Getters
    int Edge::GetLocID1(){return LocID1;}
    int Edge::GetLocID2(){return LocID2;}
    int Edge::GetToLoc(int from){return from == LocID1 ? LocID2 : LocID1;}
    int Edge::GetWeight(){return weight;}
    bool Edge::GetOpenStatus(){return openStatus;}

//Methods
    void Edge::toggleOpenStatus(){openStatus = !openStatus;}

//Constructors
    Edge::Edge(int to, int from, int distance)
    {
        LocID1 = to;
        LocID2 = from;
        weight = distance;
        openStatus = true; 
    }