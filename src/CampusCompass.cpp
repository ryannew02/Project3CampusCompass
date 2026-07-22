#include "CampusCompass.h"

#include <string>

using namespace std;

CampusCompass::CampusCompass() {
    // initialize your object
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    // return boolean based on whether parsing was successful or not
    return true;
}

bool CampusCompass::ParseCommand(const string &command) {
    // do whatever regex you need to parse validity
    // hint: return a boolean for validation when testing. For example:
    bool is_valid = true; // replace with your actual validity checking

/*
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
            N integer number of arguments to follow, should always be even as it is an interger number of edges with two locations for each edge.
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

    return is_valid;
}

