/* The Student Class (Class.h) */
#include "Student.h"
#include <iostream>
#include <algorithm>

Student::Student(int r, string n) 
{
    name = n;
    register_id = r;
    vector<Student *> adj_list = {};
}

/* Getter para a matrícula do estudante */
int Student::getRegister() const 
{
    return register_id;
}

/* Getter para o nome do estudante */
string Student::getName() const 
{
    return name;
}

/* Setter para o nome do estudante */
int Student::setName(string s)
{
    name = s;
    return 1; 
}

/* Getter na lista de adjacência */
vector<Student*>& Student::getAdjList() 
{
    return adj_list;
}

/* Adiciona o estudante s na lista de adjacência */ 
int Student::addAdjList(Student* s)
{
    adj_list.push_back(s);
    return 1;
}

/* Adiciona estudante na lista de adjacência do estudante,
 * cria elo bilateral  */
int Student::addFriend(Student* s)
{
    vector<Student*>::iterator it =
        find_if(begin(adj_list), end(adj_list),
            [=] (const Student* p) { return (p->getRegister() == s->getRegister());});
    
    if(it != adj_list.end())
        return 1;
    else
    {
        s->addAdjList(this);
        this->addAdjList(s);
    }

    return 1;
}

