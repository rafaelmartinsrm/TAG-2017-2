/* The Student Class (Class.h) */
#include <string>  
#include <vector>

using namespace std;

class Student 
{
    private:
        int register_id;
        string name;
        vector<Student*> adj_list;

    public:
        Student(int register_id, string name);
        int getRegister() const;
        string getName() const;
        int setName(string s);
        vector<Student*>& getAdjList();
        int addAdjList(Student*);
        int addFriend(Student*);
};
