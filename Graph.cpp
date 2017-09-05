#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Graph.h"

using namespace std;    

Graph::Graph(string f)
{
    fileName = f;
    result_cliques = {};
}

/* Busca estudante no vector de estudantes, se não existir, cria um. */
Student * Graph::getOrCreateStudent(int register_id, string name)
{
    vector<Student*>::iterator it = 
        find_if(begin(students), end(students), 
            [=] (const Student* p) { return (p->getRegister() == register_id);});
   
    if(it != students.end())
    {
        if((*it)->getName() == "" && name != "")
            (*it)->setName(name);

        return (*it);
    }
    else
    {
        Student *new_s = new Student(register_id, name);
        students.push_back(new_s);

        return new_s;
    }
    
}

/* X ∩ N(v), coleta a interseção entre X e os vizinhos de V */
vector<Student *> Graph::XintersectNeighborsofV(vector<Student *> R, vector<Student *> P, vector<Student *> X, vector<Student *> v)
{
    vector<Student *> Xtemp = {};
    for(vector<Student *>::size_type i = 0; i < X.size(); i++)
    {
        for(vector<Student *>::size_type j = 0; j < v.back()->getAdjList().size(); j++)
        {
            if(X[i] == v.back()->getAdjList()[j])
                Xtemp.push_back(X[i]);
        }
    }
    return Xtemp;
}

/* P ∩ N(v), coleta a interseção entre P e os vizinhos de V */
vector<Student *> Graph::PintersectNeighborsofV(vector<Student *> R, vector<Student *> P, vector<Student *> X, vector<Student *> v)
{
    vector<Student *> Ptemp = {};
    for(vector<Student*>::size_type i = 0; i < P.size(); i++)
    {
        for(vector<Student *>::size_type j = 0; j < v.back()->getAdjList().size(); j++)
        {
            if(P[i] == v.back()->getAdjList()[j])
                Ptemp.push_back(P[i]);
        }
    }

    return Ptemp;
}

vector<Student *> Graph::XunionV(vector<Student *> R, vector<Student *> P, vector<Student *> X, vector<Student *> v)
{
    vector<Student *> Xtemp;
    Xtemp = X;
    Xtemp.push_back(v.back());

    return Xtemp;
}
/* P \ V, remove o vértice V do vetor P */
vector<Student *> Graph::PremoveV(vector<Student *> R, vector<Student *> P, vector<Student *> X, vector<Student *> v)
{
    vector<Student *> Ptemp;
    Ptemp = P;
    for(vector<Student*>::size_type i = 0; i < Ptemp.size(); i++)
    {
        if(Ptemp[i] == v.back())
            Ptemp.erase(Ptemp.begin() + i);
    }

    return Ptemp;
}

/* R U V, faz a união entre R e V */
vector<Student *> Graph::RunionV(vector<Student *> R, vector<Student *> P, vector<Student *> X, vector<Student *> v)
{
    vector<Student *> Rtemp;
    Rtemp = R;
    Rtemp.push_back(v.back());

    return Rtemp;
}

/*
 * Implementa o algoritmo abaixo:
 *
 * WITHOUT PIVOTING:
 *
 * BronKerbosch1(R, P, X) :
 *  if P and X are both empty :
 *  report R as a maximal clique
 *  for each vertex v in P :
 *      BronKerbosch1(R U{ v }, P n N(v), X n N(v))
 *          P : = P \ {v}
 *              X: = X U {v}
 *              
 *              */
int Graph::BronKerbosch(vector<Student *> R, vector<Student *> P, vector<Student *> X)
{
    if(P.empty() && X.empty())
    {
        result_cliques.push_back(R);
    }

    for(vector<Student *>::size_type i = 0; i < P.size(); i++)
    {
        vector<Student*> v_ = {};
        v_.insert(v_.end(), P[i]);

        BronKerbosch(RunionV(R, P, X, v_), PintersectNeighborsofV(R, P, X, v_), XintersectNeighborsofV(R, P, X, v_));
        
        P = PremoveV(R, P, X, v_);
        X = XunionV(R, P, X, v_); 
    
        if (P.empty())
            return 1;
        else
            i = 0;
        
    }

    return 1;
}

/* Imprime o máximo e maximal do vetor de estudantes do grafo */
int Graph::printMaxes()
{
    int max = 0;
    for(vector <Student*>::size_type i = 0; i < result_cliques.size(); i++)
    {
        if(result_cliques[max].empty())
            max = i;
        else if(result_cliques[max].size() < result_cliques[i].size())
            max = i;
    }
    cout << "MÁXIMO: " << endl;
    for(vector <Student*>::size_type i = 0; i < result_cliques[max].size(); i++)
    {
        cout << result_cliques[max][i]->getName();
        if(i+1 != result_cliques[max].size())
            cout << ", ";
    }
    result_cliques.erase(result_cliques.begin() + max);
    cout << "\n" << endl;

    cout << "MAXIMAL: " << endl;
    int maximal = 0;
    for(vector <Student*>::size_type i = 0; i < result_cliques[maximal].size(); i++)
    {
        cout << result_cliques[maximal][i]->getName();
        if(i+1 != result_cliques[maximal].size())
            cout << ", ";
    }

    cout << "\n" << endl;
    return 1;
}

/* Define a comparação entre estudantes para o sorter
 *   Compara o tamanho entre as listas de adjacência */
bool Graph::cmp(Student * a, Student * b)
{
    return a->getAdjList().size() > b->getAdjList().size();
};

/* Organiza a lista de estudantes por tamanho da lista de 
 * adjacência de forma decrescente */
int Graph::studentsSorter()
{
    sort(students.begin(), students.end(), cmp);
    return 1;
}
/* Coleta o arquivo de entrada e trata ele para inserir
 * no vetor de estudantes, formando o grafo */
int Graph::parseFile()
{
    ifstream ifs;
    ifs.open(fileName, std::ifstream::in);

    string line;
    vector<string> result;

    char delim = '|';

    if(ifs.is_open())
    {
        while(getline(ifs, line))
        {
            auto i = 0;
            auto pos = line.find(delim);
            while(pos != string::npos)
            {
                result.push_back(line.substr(i, pos-i));
                i = ++pos;
                pos = line.find(delim, pos); 

                if(pos == string::npos)
                    result.push_back(line.substr(i, line.length()));
            }
            /* Creates Students */
            int register_id = atoi(result.at(1).c_str());
            Student* cur_s = getOrCreateStudent(register_id, result.at(0));

            /* Adds friends (if) to adj_list */
            for(int i = 2; i < result.size(); ++i)
            {
                Student* cur = getOrCreateStudent(atoi(result.at(i).c_str()), "");
                cur_s->addFriend(cur);
            }
            result.clear();
        }
    }
    studentsSorter(); 
    ifs.close();
    return 1;
}

/* Getter para o vetor de estudantes */
vector<Student *>& Graph::getStudents()
{
    return this->students;
}

int main()
{
    Graph g("amigos_20172.txt");
    g.parseFile();
    vector<Student*> R, P, X;

    for (Student* i : g.getStudents())
    {
        P.push_back(i);
        cout << i->getRegister() << " : " << i->getAdjList().size() << endl;
    }

    g.BronKerbosch(R, P, X);
    g.printMaxes();
    return 0;
}
