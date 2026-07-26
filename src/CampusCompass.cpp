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
        "toggleEdgesClosure", "checkEdgeStatus", "isConnected", "printShortestEdges", "printStudentZone", "verifyScheudle"};
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
            if(stoi(commandParams[4]) != paramNum){cout << "unsuccessful" << endl;return false;};
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
            if(stoi(commandParams[1]) != (paramNum - 2)/2){cout << "unsuccessful" << endl; return false;}
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
            if(commandParams.size() != 3){cout << "unsuccessful" << endl; return false;}
            return checkEdge(make_pair(stoi(commandParams[1]), stoi(commandParams[2])));
        }            
            
        case 7:
//#isConnected LOCATION_ID_A LOCATION_ID_B ;
        {
            if(commandParams.size() != 3){cout << "unsuccessful" << endl; return false;}
            return checkRoute(make_pair(stoi(commandParams[1]), stoi(commandParams[2])));
        }   

        case 8:
//#printShortestEdges STUDENT_ID ;
            return true;
            
        case 9:
//#printStudentZone STUDENT_ID ;
            return true;
            
        case 10:
//#verifySchedule ID ; 
            return true;
    }
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
    return(students[studentID].removeCourse(courseCode) && catalog[courseCode].removeFromRoster(studentID));
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
        if(edgeList[i].GetOpenStatus()){cout << "Open" << endl; return true;}
        else{cout << "Closed" << endl; return false;}
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
{return {};}
int CampusCompass::dispalyStudentZone(int studentID)
{return 0;}
int CampusCompass::verifySchedule(int studentID)
{return 0;}

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
/*
    ONLY LOCATION DATA NEEDS TO BE REPRESENTED AS A GRAPH

    Valid commands:
    ###=== Note Sections (***Note) <- may be good test cases to implement for incorrect input and or functionality testing. ===###
       === Students are limited to 6 classes and must be removed from the structure if they have 0 classes enrolled. ===
       === Any commands not meeting the requirements for formatting must be ignored and "unsuccessful" printed ===
       === The first line of input will denote how many inputs are to follow see https://ufl.instructure.com/courses/570816/assignments/7096026 for examples === 

        #insert STUDENT_NAME STUDENT_ID RESIDENCE_LOCATION_ID N CLASSCODE_1 CLASSCODE_2 ... CLASSCODE_N
            STUDENT_NAME is [a-z, A-Z, /s] wrapped in quotes. I.E. "John Smith".
            STUDENT_ID is [0-9] 8 digits exactly and must be unique.
            RESIDENCE_LOCATION_ID must be a valid location ID.
            N integer number of arguments to follow, number of classcodes.
            CLASSCODE... [A-Z] 3 characters immediatly followed by [0-9] 4 digits exactly.
            ***Note: N must match the number of args passed as classcodes preceeding N or the input is invalidated!
            $Desc: creates and inserts a new students record, prints "successful" or "unsuccessful"

        #remove STUDENT_ID
            STUDENT_ID is [0-9] 8 digits exactly and must be unique
            ***Note: STUDENT_ID must exist.
            $Desc: deletes a students record, prints "successful" or "unsuccessful"

        #dropClass STUDENT_ID CLASSCODE
            STUDENT_ID is [0-9] 8 digits exactly and must be unique
            CLASSCODE [A-Z] 3 characters immediatly followed by [0-9] 4 digits exactly.
            ***Note: If this is the students only class this should also recursivly remove the student.
            ***Note: Fails is Student is not enrolled in classcode, if classcode does not exist, and if student doesnt exist.
            $Desc: drops a students class, prints "successful" or "unsuccessful"
        
        #replaceClass STUDENT_ID CLASSCODE_A CLASSCODE_B
            STUDENT_ID is [0-9] 8 digits exactly and must be unique
            CLASSCODE [A-Z] 3 characters immediatly followed by [0-9] 4 digits exactly.
            ***Note: Should fail if student or either classcode does not exist.
            ***Note: Should fail if student is already enrolled in CLASSCODE_B or is not enrolled in CLASSCODE_A.
            $Desc: replaces a students class, prints "successful" or "unsuccessful"
        
        #removeClass CLASSCODE 
            CLASSCODE [A-Z] 3 characters immediatly followed by [0-9] 4 digits exactly.
            ***Note: Fails if classcode does not exist, is formated wrong, or no students are enrolled in the class.
            $Desc: Removes a class from the schedule recursivly dropping all students and prints the number of students that were dropped. 
        
        #toggleEdgesClosure N LOCATION_ID_A LOCATION_ID_B ... LOCATION_ID_X LOCATION_ID_Y
            2N integer number of arguments to follow, should always be even as it is an interger number of edges with two locations for each edge.
            LOCATION_ID... integer representing location ID, these will always come in pairs
            ***Note: This is a toggle so edgeClosed = !edgeClosed I.E (if open now shut...if shut now open)
            ***Note: Edges given will always be valid!!!!
            $Desc: toggles the closure status of given set of edges, prints successful on completion

        #checkEdgeStatus LOCATION_ID_A LOCATION_ID_B
            LOCATION_ID... integer representing location ID, these will always come in pairs
            $Desc: checks the status of an edge, prints "DNE" if the edge does not exist, otherwise it prints "open" or "closed" according to its current status.

        #isConnected LOCATION_ID_A LOCATION_ID_B
            LOCATION_ID... integer representing location ID, these will always come in pairs
            $Desc: Checked connectedness between two locations, prints "unsuccessful" if there is no path between the locations otherwise prints "successful"

        #printShortestEdges STUDENT_ID
            STUDENT_ID is [0-9] 8 digits exactly and must be unique
            $Desc: Prints the shortest path (total weights) directly from the students residence to each of their classes currently enrolled in ignoring "closed" edges.
                    EXAMPLE: 
                    RYAN JUERGENS
                    BSC2005: 52
                    CIS3100: 8
                    COP3530: 14
            ***Note: if a class cannot be reached the time should be -1
            ***Note: classes should be printed in lexographical order a->b->c...1->2->3...
        
        #printStudentZone STUDENT_ID
            STUDENT_ID is [0-9] 8 digits exactly and must be unique
            $Desc: Find the shortest path (total weights) directly from the students residence to each of their classes currently enrolled in ignoring "closed" edges.
                Union these paths to form a subset of the original graph.
                Find the Minimum Spanning tree of this sub-graph.
                EXAMPLE:
                Student Zone Cost For RYAN JUERGENS: 60
            ***Note: No validation is need as to whether classes are reachable, no student will be passed that currently has unreachable classes due to Edge closures. 
        
        #verifySchedule ID
            STUDENT_ID is [0-9] 8 digits exactly and must be unique
            $Desc: Find the shortest path between each of the students classes in chronological order and compare that to the actual time available between the classes ignoring "closed" edges.
                    EXAMPLE: 
                    {DATA}
                    {Students classes are COP3501: 10:00 - 10:55 ; COP3530: 11:15 - 12:20 ; BSC2005: 12:30 - 1:45}
                    &
                    {Shortest paths are COP3501 -> COP3530: 15 and COP3530 -> BSC2005: 20}
                    OUTPUT:
                    Schedule Check for RYAN JUERGENS:
                    COP3501 - COP3530: successful
                    COP3530 - BSC2005: unsuccessful
            ***Note: if a student has only one class print "unsuccessful"
*/


