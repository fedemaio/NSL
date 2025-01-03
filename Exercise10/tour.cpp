/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <utility>
#include <sstream>
#include <vector>
#include "random.h"
#include "tour.h"

using namespace std;

//////////////////////////////////////////////////////////////////////constructors and distructors/////////////////////////////////////////////////////////////////////////////////////////
city :: city(int idx, double xcoord, double ycoord){
    index = idx;
    x = xcoord;
    y = ycoord;
}

city :: city(){}

tour :: tour(){
    
}

tour :: ~tour(){}

tour :: tour(vector<city> c){
    _cities = c;
    _num_cities = c.size();
}

////////////////////////////////////////////////////////////////////tour functions///////////////////////////////////////////////////////////////////////////////////////////////
//sets a whole vector of cities
void tour :: set_cities(vector<city> c){
    _cities = c;
}

//sets a single city
void tour :: set_city(int n, city c){
    _cities[n] = c;
}

//returns the ith city
city tour :: operator[](int i){
    return _cities[i];
}

int tour :: get_num_cities(){
    return _num_cities;
}

//check function
bool tour :: check(){
    bool flag;

    //check to start from the first city (set at 1)
    if(_cities[0].index != 0){
        cerr << "Il tour parte dalla città sbagliata" << endl;
        return false;
    }

    //check to tour all cities
    for(int i=0; i<_num_cities; i++){
        flag = false;
        for(int j=0; j<_num_cities; j++){
            if(_cities[j].index == i) flag = true;
        }
        if(!flag){
            cerr << "Il tour non visita tutte le città" << endl;
            return false;
        }
    }
    
    return false;
}

//calculation of the path lenght
double tour :: path_lenght(){
    double L = 0.0;
    for(int i=0; i<_num_cities-1; i++){
        L += sqrt(pow(_cities[i].x - _cities[i+1].x, 2) + pow(_cities[i].y - _cities[i+1].y, 2));
    }
    return L + sqrt(pow(_cities[_num_cities-1].x - _cities[0].x, 2) + pow(_cities[_num_cities-1].y - _cities[0].y, 2));
}

//it prints out all cities index
void tour :: print(){
    for(int i=0; i<_num_cities; i++){
        cout << _cities[i].index << "  ";
    }
    cout << endl;
}

//returns a vector with city index
vector<int> tour :: serialize(){
    vector<int> data;
    for(int i=0; i<_num_cities; i++){
        data.push_back(_cities[i].index);
    }
    return data;
}

//orders _cities vector with data sequence given in input
void tour :: deserialize(const vector<int> &data){
    if (data.size() != _num_cities) {
        cerr << "Data size does not match number of cities.";
    } 
    vector<city> new_cities(_num_cities);
    for(int i=0; i<_num_cities; ++i) {
        for(int j=0; j<_num_cities; ++j) {
            if (data[i] == _cities[j].index) {
                new_cities[i] = _cities[j];
            }
        }
    }
    for(int i=0; i<_num_cities; i++){
        _cities[i] = new_cities[i];
    }
}
/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/