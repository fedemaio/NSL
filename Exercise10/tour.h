/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
#ifndef __tour__
#define __tour__
#include <iostream>
#include <fstream>
#include <vector>
#include "random.h"

using namespace std;

struct city {
    int index;
    double x;
    double y;

    city(int idx, double xcoord, double ycoord);
    city();
};

class tour {

    private:
    vector<city> _cities; //vettore contenente le città che stanno nel tour
    int _num_cities; //numero di città nel tour

    public:
    tour();
    tour(vector<city>);
    ~tour();

    void set_cities(vector<city>);
    void set_city(int, city);

    city operator[](int); //operatore che ritorna l'i-esima città del tour
    int get_num_cities();
    bool check(); //controlla che il tour rispetti le condizioni
    double path_lenght(); //restituisce la lunghezza del tour (metrica L^2)
    void print();
    //added in ex 10
    vector<int> serialize();
    void deserialize(const vector<int>&);
    
};

#endif
/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/