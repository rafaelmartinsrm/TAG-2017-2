#include <iostream>
#include <list>
#include "Student.h"
/* Receives a file and returns a list of students */

using namespace std;

class Graph
{
    private:
        vector<Student *> students;
        string fileName;
        vector< vector<Student *> > result_cliques;

    public:
        Graph(string file_location);
        int parseFile();
        Student * getOrCreateStudent(int register_id, string name); 
        int studentsSorter();
        bool static cmp (Student*, Student*); 
        int BronKerbosch(vector<Student *>, vector<Student *>, vector<Student *>);
        int printMaxes();
        vector<Student *>& getStudents();
        vector<Student *> XintersectNeighborsofV(vector<Student *>, vector<Student *>, vector<Student *>, vector<Student *>);
        vector<Student *> PintersectNeighborsofV(vector<Student *>, vector<Student *>, vector<Student *>, vector<Student *>);
        vector<Student *> XunionV(vector<Student *>, vector<Student *>, vector<Student *>, vector<Student *>);
        vector<Student *> PremoveV(vector<Student *>, vector<Student *>, vector<Student *>, vector<Student *>);
        vector<Student *> RunionV(vector<Student *>, vector<Student *>, vector<Student *>, vector<Student *>);
};
