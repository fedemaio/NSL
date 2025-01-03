/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
#ifndef __population__
#define __population__
#include "tour.h"
using namespace std;

class population {

    private:
    vector<tour> _tours;
    Random _rnd;
    int _num_cities;
    //vector<tour> _tours; //contiene tutti i tour della popolazione
    int _num_chrom; //number of chromosomes
    float _p_c; //probability of crossover
    float _p_m_1; //probability of mutation
    float _p_m_2;
    float _p_m_3;

    void _mutate_1(tour&); 
    void _mutate_2(tour&);
    void _mutate_3(tour&);
    void _crossover(tour&, tour&);
    bool _is_same(tour, tour); //checks if two tours are the same
    tour _select(); //returns the tour which fits the most (based on probability)

    public:
    population(Random&, int, tour, float, float, float, float);
    ~population();
    
    int get_num_chrom(); 
    void step(); 
    void print();
    tour get_best_tour(); //shortest tour
    double half_AV();
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