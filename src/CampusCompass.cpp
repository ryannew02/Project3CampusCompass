#include "CampusCompass.h"
#include "Edge.h"
#include "Course.h"
#include "Student.h"

using namespace std;

CampusCompass::CampusCompass()
    {
        adjacencyList = {};
        edgeList = {};
        students = {};
        catalog = {};
        vector<string> tempCommandsPossible = {"insert", "remove", "dropClass", "replaceClass", "removeClass",
        "toggleEdgesClosure", "checkEdgeStatus", "isConnected", "printShortestEdges", "printStudentZone", "verifySchedule"};
        for(unsigned int i = 0; i < tempCommandsPossible.size(); i++){commandsPossible[tempCommandsPossible[i]] = i;}
    };

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    ifstream edgeFile(edges_filepath);
    //LocationID_1,LocationID_2,Name_1,Name_2,Time
    ifstream classFile(classes_filepath);
    //ClassCode,LocationID,Start Time (HH:MM),End Time (HH:MM)

    //Load Edges into graph
    if(!edgeFile.is_open() || !classFile.is_open()){return false;}
    string line;
    getline(edgeFile, line);
    vector<string> readData;
    int edgeID = 0; //edge ID is assigned to both directions of the edge to ensure both directions are toggled together
    while(getline(edgeFile, line))
    {
        readData.clear();
        stringstream lineStream(line);
        while(getline(lineStream, line, ','))
        {readData.push_back(line);}
        Edge tempEdge = Edge(stoi(readData[0]), stoi(readData[1]), stoi(readData[4]));
        validResidences.insert(stoi(readData[0]));
        validResidences.insert(stoi(readData[1]));
        edgeList.push_back(tempEdge);
        adjacencyList[stoi(readData[0])].push_back(edgeID);
        adjacencyList[stoi(readData[1])].push_back(edgeID);
        edgeID++;
    }

    //Load courses into catalog
    getline(classFile, line);
    while(getline(classFile, line))
    {
        readData.clear();
        stringstream lineStream(line);
        while(getline(lineStream, line, ','))
        {readData.push_back(line);}
        string hours;
        string minutes;
        stringstream startStream(readData[2]);
        getline(startStream, hours, ':');
        getline(startStream, minutes, ':');
        int tempStart = stoi(hours) * 60 + stoi(minutes);
        stringstream endStream(readData[3]);
        getline(endStream, hours, ':');
        getline(endStream, minutes, ':');
        int tempEnd = stoi(hours) * 60 + stoi(minutes);
        catalog[readData[0]] = Course(readData[0], stoi(readData[1]), tempStart, tempEnd);   
    }

    return true;
}

bool CampusCompass::ParseCommand(const string &command) { 
    vector<string> commandParams = splitCommand(command);
    if(!commandsPossible.count(commandParams[0])){cout << "unsuccessful" << endl;return false;}
    int commandNum = commandsPossible[commandParams[0]];
    switch (commandNum)
    {
        case 0:
        {
//#insert STUDENT_NAME STUDENT_ID RESIDENCE_LOCATION_ID N CLASSCODE_1 CLASSCODE_2 ... CLASSCODE_N
            
            //ensure courses to add between 1-6.
            if(commandParams.size() < 6 || commandParams.size() > 11){cout << "unsuccessful" << endl;return false;;}
            //validate details
            if(!validateName(commandParams[1]) || !validateUFID(commandParams[2]) || !validateResidenceID(commandParams[3]) || students.count(stoi(commandParams[2])) > 0){cout << "unsuccessful" << endl;return false;}
            int paramNum = commandParams.size() - 5;
            //validate course count and courses passed are correct
            if(!isNumber(commandParams[4]) || stoi(commandParams[4]) != paramNum){cout << "unsuccessful" << endl;return false;};
            //create student record
            vector<string> courseSchedule;
            for(unsigned int i = 5; i < commandParams.size(); i++){courseSchedule.push_back(commandParams[i]);}
            //fail if course doesnt exist
            bool allCoursesExist = true;
            set<string> duplicatesCheck;
            for(auto i : courseSchedule)
            {
                duplicatesCheck.insert(i);
                if(catalog.count(i) == 0)
                {
                    cout << "unsuccessful" << endl;
                    allCoursesExist = false;
                    break;
                }
            }
            if(!allCoursesExist){return false;}
            if(duplicatesCheck.size() != (commandParams.size() - 5)){cout << "unsuccessful" << endl; return false;}
            //create student record and add them to appropriate rosters
            students[stoi(commandParams[2])] = Student(commandParams[1], stoi(commandParams[2]), stoi(commandParams[3]), courseSchedule);
            for(auto i : courseSchedule){catalog[i].addToRoster(stoi(commandParams[2]));}
            cout << "successful" << endl;
            return true;
        }

        case 1:
        {
//#remove STUDENT_ID

            if(commandParams.size() != 2 || !validateUFID(commandParams[1])){cout << "unsuccessful" << endl;return false;}
            if(!removeStudent(stoi(commandParams[1]))){cout << "unsuccessful" << endl; return false;}
            cout << "successful" << endl;
            return true;
        }

        case 2:
//#dropClass STUDENT_ID CLASSCODE
        {
            if(commandParams.size() != 3 || !validateUFID(commandParams[1])){cout << "unsuccessful" << endl; return false;}
            if(!dropStudent(stoi(commandParams[1]), commandParams[2])){cout << "unsuccessful" << endl; return false;}
            cout << "successful" << endl;
            return true;
        }

        case 3:
//#replaceClass STUDENT_ID CLASSCODE_A CLASSCODE_B ;
        {
            if(commandParams.size() != 4 || !validateUFID(commandParams[1])){cout << "unsuccessful" << endl;return false;}
            if(!swapStudent(stoi(commandParams[1]), commandParams[2], commandParams[3])){cout << "unsuccessful" << endl; return false;}
            cout << "successful" << endl;
            return true;
        }

        case 4:
//#removeClass CLASSCODE
        {
            if(commandParams.size() != 2 || catalog.count(commandParams[1]) == 0){cout << "unsuccessful" << endl; return false;}
            vector<int> studentRoster = catalog[commandParams[1]].GetRoster();
            if(studentRoster.empty()){cout << "unsuccessful" << endl; return false;}
            catalog.erase(commandParams[1]);
            for(auto id : studentRoster){
                students[id].removeCourse(commandParams[1]);
                if(students[id].GetCourseSchedule().empty()){students.erase(id);}
            }
            cout << studentRoster.size() << endl;
            return true;
        }

        case 5:
//#toggleEdgesClosure N LOCATION_ID_A LOCATION_ID_B ... LOCATION_ID_X LOCATION_ID_Y
        {
            if(commandParams.size() < 4 || (commandParams.size() % 2 != 0)){cout << "unsuccessful" << endl; return false;}
            int paramNum = commandParams.size();
            if(!isNumber(commandParams[1]) || stoi(commandParams[1]) != (paramNum - 2)/2){cout << "unsuccessful" << endl; return false;}
            vector<pair<int, int>> edges;
            //check each location exists
            for( int i = 2; i < paramNum; i++){if(!validateResidenceID(commandParams[i])){cout << "unsuccessful" << endl; return false;}}
            //add to vector
            for( int i = 2; i < paramNum; i += 2){edges.push_back(make_pair(stoi(commandParams[i]), stoi(commandParams[i+1])));}
            return closeEdges(edges);
        }
            
        case 6:
//#checkEdgeStatus LOCATION_ID_A LOCATION_ID_B ; 
        {
            if(commandParams.size() != 3 || !isNumber(commandParams[1]) || !isNumber(commandParams[2])){cout << "unsuccessful" << endl; return false;}
            return checkEdge(make_pair(stoi(commandParams[1]), stoi(commandParams[2])));
        }            
            
        case 7:
//#isConnected LOCATION_ID_A LOCATION_ID_B ;
        {
            if(commandParams.size() != 3 || !isNumber(commandParams[1]) || !isNumber(commandParams[2])){cout << "unsuccessful" << endl; return false;}
            return checkRoute(make_pair(stoi(commandParams[1]), stoi(commandParams[2])));
        }   

        case 8:
//#printShortestEdges STUDENT_ID ;
        {
            if(commandParams.size() != 2 || !validateUFID(commandParams[1]) || students.count(stoi(commandParams[1])) == 0){cout << "unsuccessful" << endl; return false;}
            int UFID = stoi(commandParams[1]);
            cout << "Time For Shortest Edges: " << students[UFID].GetName() << endl;
            for(auto& route : displayAvailableRoutes(UFID)){
                cout << route.first << ": " << route.second << endl;
            }
            return true;
        }

        case 9:
//#printStudentZone STUDENT_ID
        {
            if(commandParams.size() != 2 || !validateUFID(commandParams[1]) || students.count(stoi(commandParams[1])) == 0){cout << "unsuccessful" << endl; return false;}
            int UFID = stoi(commandParams[1]);
            cout << "Student Zone Cost For " << students[UFID].GetName() << ": " << dispalyStudentZone(UFID) << endl;
            return true;
        }
        
        case 10:
//#verifySchedule ID
        {
            if(commandParams.size() != 2 || !validateUFID(commandParams[1]) || students.count(stoi(commandParams[1])) == 0){cout << "unsuccessful" << endl; return false;}
            verifySchedule(stoi(commandParams[1]));
            return true;
        }
    }
    cout << "unsuccessful" << endl;
    return false;
}

//Getters
map<int, vector<int>> CampusCompass::GetAdjacencyList(){return adjacencyList;}
vector<Edge> CampusCompass::GetEdgeList(){return edgeList;}
map<int, Student> CampusCompass::GetStudents(){return students;}
map<string, Course> CampusCompass::GetCatalog(){return catalog;}

//Modifiers

bool CampusCompass::removeStudent(int studentID)
{
    if(students.count(studentID) == 0){return false;}
    vector<string> courses = students[studentID].GetCourseSchedule();
    students.erase(studentID);
    for(auto i : courses){catalog[i].removeFromRoster(studentID);}
    return true;
}

bool CampusCompass::dropStudent(int studentID, string courseCode)
{
    if(students.count(studentID) == 0){return false;}
    if(catalog.count(courseCode) == 0){return false;}
    if(!students[studentID].isEnrolledIn(courseCode)){return false;}
    bool success = students[studentID].removeCourse(courseCode) && catalog[courseCode].removeFromRoster(studentID);
    students[studentID].GetCourseSchedule().size() == 0 ? students.erase(studentID) : success;
    return success;
}

bool CampusCompass::swapStudent(int studentID, string courseCodeA, string courseCodeB)
{
    if(students.count(studentID) == 0){return false;}
    if(catalog.count(courseCodeA) == 0 || catalog.count(courseCodeB) == 0){return false;}
    if(!students[studentID].isEnrolledIn(courseCodeA)){return false;}
    if(students[studentID].isEnrolledIn(courseCodeB)){return false;}
    if(!catalog[courseCodeA].isEnrolledIn(studentID)){return false;}
    if(catalog[courseCodeB].isEnrolledIn(studentID)){return false;}
    return(students[studentID].removeCourse(courseCodeA) &&
     catalog[courseCodeA].removeFromRoster(studentID) &&
      students[studentID].addCourse(courseCodeB) &&
       catalog[courseCodeB].addToRoster(studentID));
}

bool CampusCompass::closeEdges(vector<pair<int, int>> LocationIDs)
{
    for(auto edge : LocationIDs)
    {
        vector<int> possibleEdges = adjacencyList[edge.first];
        vector<int> otherPossibleEdges = adjacencyList[edge.second];
        for(auto i : possibleEdges){for(auto j : otherPossibleEdges){if(i == j){edgeList[i].toggleOpenStatus();break;}}}
    }
    cout << "successful" << endl;
    return true;
}
//Reporting
bool CampusCompass::checkEdge(pair<int, int> LocationIDs)
{
    if(adjacencyList.count(LocationIDs.first) == 0 || adjacencyList.count(LocationIDs.second) == 0)
    {
        cout << "DNE" << endl;
        return false;
    }    
    vector<int> possibleEdges = adjacencyList[LocationIDs.first];
    vector<int> otherPossibleEdges = adjacencyList[LocationIDs.second];
    for(auto i : possibleEdges){for(auto j : otherPossibleEdges){if(i == j)
        {
        if(edgeList[i].GetOpenStatus()){cout << "open" << endl; return true;}
        else{cout << "closed" << endl; return false;}
        }}}
    cout << "DNE" << endl;
    return false;
}

bool CampusCompass::checkRoute(pair<int, int> LocationIDs)
{
    int start = LocationIDs.first;
    int target = LocationIDs.second;

    if(adjacencyList.count(start) == 0 || adjacencyList.count(target) == 0){cout << "unsuccessful" << endl; return false;}
    if(start == target){cout << "successful" << endl; return true;}
    set<int> visited;
    queue<int> toVisit;
    toVisit.push(start);
    visited.insert(start);
    while(!toVisit.empty())
    {
        int current = toVisit.front();
        toVisit.pop();
        for(int edgeID : adjacencyList[current])
        {
            if(!edgeList[edgeID].GetOpenStatus()){continue;}
            int neighbor = edgeList[edgeID].GetToLoc(current);
            if(neighbor == target){cout << "successful" << endl; return true;}
            if(visited.count(neighbor) == 0)
            {
                visited.insert(neighbor);
                toVisit.push(neighbor);
            }
        }
    }
    cout << "unsuccessful" << endl;
    return false;
}

vector<pair<string, int>> CampusCompass::displayAvailableRoutes(int studentID)
{
    vector<pair<string, int>> results;
    if(students.count(studentID) == 0){return results;}

    Student& student = students[studentID];
    map<int, pair<int, int>> distances = findDijkstraTable(student.GetResidenceID());

    for(auto& courseCode : student.GetCourseSchedule())
    {
        if(catalog.count(courseCode) == 0){continue;}
        int loc = catalog[courseCode].GetLocID();
        int dist = distances.count(loc) ? distances[loc].first : -1;
        results.push_back({courseCode, dist});
    }
    sort(results.begin(), results.end()); 
    return results;
}

int CampusCompass::dispalyStudentZone(int studentID)
{
    if(students.count(studentID) == 0){return -1;}
    Student& student = students[studentID];
    int source = student.GetResidenceID();

    map<int, pair<int,int>> dTable = findDijkstraTable(source);

    set<int> nodeSet;
    nodeSet.insert(source);
    for(auto& courseCode : student.GetCourseSchedule())
    {
        if(catalog.count(courseCode) == 0){continue;}
        int target = catalog[courseCode].GetLocID();
        if(dTable.count(target) == 0){continue;}
        int cur = target;
        while(cur != source)
        {
            nodeSet.insert(cur);
            int predEdge = dTable[cur].second;
            cur = edgeList[predEdge].GetToLoc(cur);
        }
    }

    vector<int> candidateEdges;
    set<int> seenEdgeIDs;
    for(int node : nodeSet)
    {
        if(adjacencyList.count(node) == 0){continue;}
        for(int edgeID : adjacencyList[node])
        {
            if(!edgeList[edgeID].GetOpenStatus()){continue;}
            if(seenEdgeIDs.count(edgeID)){continue;}
            int other = edgeList[edgeID].GetToLoc(node);
            if(nodeSet.count(other))
            {
                candidateEdges.push_back(edgeID);
                seenEdgeIDs.insert(edgeID);
            }
        }
    }

    sort(candidateEdges.begin(), candidateEdges.end(), [this](int a, int b){
        return edgeList[a].GetWeight() < edgeList[b].GetWeight();
    });

    map<int,int> parent;
    for(int node : nodeSet){parent[node] = node;}

    int totalCost = 0;
    for(int edgeID : candidateEdges)
    {
        int u = edgeList[edgeID].GetLocID1();
        int v = edgeList[edgeID].GetLocID2();
        int rootU = find(parent, u);
        int rootV = find(parent, v);
        if(rootU != rootV)
        {
            parent[rootU] = rootV;
            totalCost += edgeList[edgeID].GetWeight();
        }
    }
    return totalCost;
}

int CampusCompass::verifySchedule(int studentID)
{
    Student& student = students[studentID];
    vector<string> schedule = student.GetCourseSchedule();

    cout << "Schedule Check for " << student.GetName() << ":" << endl;

    if(schedule.size() <= 1){cout << "unsuccessful" << endl; return 0;}

    sort(schedule.begin(), schedule.end(), [this](const string& a, const string& b){
        return catalog[a].GetStartTime() < catalog[b].GetStartTime();
    });

    for(size_t i = 0; i + 1 < schedule.size(); i++)
    {
        string codeA = schedule[i];
        string codeB = schedule[i+1];

        int buffer = catalog[codeA].GetTimeBuffer(catalog[codeB]); // B.start - A.end
        int locA = catalog[codeA].GetLocID();
        int locB = catalog[codeB].GetLocID();

        map<int, pair<int,int>> dTable = findDijkstraTable(locA);
        bool reachable = dTable.count(locB) > 0;
        bool ok = reachable && dTable[locB].first <= buffer;

        cout << codeA << " - " << codeB << ": " << (ok ? "successful" : "unsuccessful") << endl;
    }
    return 0;
}

//Helpers
// STUDENT_NAME only upper and lower alpha characters
bool CampusCompass::validateName(string param) {
    static const regex namePattern(R"(^[A-Za-z]+( [A-Za-z]+)*$)");
    return regex_match(param, namePattern);
}

// STUDENT_ID exactly 8 digits.
bool CampusCompass::validateUFID(string param) {
    static const regex ufidPattern(R"(^\d{8}$)");
    return regex_match(param, ufidPattern);
}

// RESIDENCE_LOCATION_ID must exist in the set.
bool CampusCompass::validateResidenceID(string param) {
    static const regex intPattern(R"(^\d+$)");
    if (!regex_match(param, intPattern)) return false;
    return validResidences.count(stoi(param)) > 0;
}

//parse each command and extract name properly
vector<string> CampusCompass::splitCommand(string line) {
    vector<string> tokens;
    string current;
    bool inQuotes = false;
    for (char c : line) 
    {
        if (c == '"') 
        {
            inQuotes = !inQuotes;
            continue;
        }
        if (c == ' ' && !inQuotes) 
        {
            if (!current.empty()) 
            {
                tokens.push_back(current);
                current.clear();
            }
            continue;
        }
        current += c;
    }
    if (!current.empty()) {
        tokens.push_back(current);
    }
    return tokens;
}

map<int, pair<int, int>> CampusCompass::findDijkstraTable(int source)
{
    map<int, pair<int, int>> distances;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> prioQuene;
    distances[source] = make_pair(0, -1);
    prioQuene.push(make_pair(0, source));
    while(!prioQuene.empty())
    {
        int x = prioQuene.top().first;
        int y = prioQuene.top().second;
        prioQuene.pop();
        if(x > distances[y].first){continue;}
        for(int edgeID : adjacencyList[y])
        {
            if(!edgeList[edgeID].GetOpenStatus()){continue;}
            int z = edgeList[edgeID].GetToLoc(y);
            int newDist = x + edgeList[edgeID].GetWeight();
            if(distances.count(z) == 0 || newDist < distances[z].first)
            {
                distances[z].first = newDist;
                distances[z].second = edgeID;
                prioQuene.push({newDist, z});
            }
        }
    }
    return distances;
}

int CampusCompass::find(map<int,int>& parent, int x)
{
    if(parent[x] != x){parent[x] = find(parent, parent[x]);}
    return parent[x];
}

bool CampusCompass::isNumber(string param) {
    static const regex intPattern(R"(^\d{1,9}$)");
    return regex_match(param, intPattern);
}
