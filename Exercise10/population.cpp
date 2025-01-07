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
#include <random>
#include <algorithm>
#include "random.h"
#include "population.h"

using namespace std;

population :: ~population(){}

population :: population(Random& rnd, int n, tour t, float pc, float pm1, float pm2, float pm3){
    _rnd = rnd; 
    _num_chrom = n;
    _p_c = pc;
    _p_m_1 = pm1;
    _p_m_2 = pm2;
    _p_m_3 = pm3;
    _num_cities = t.get_num_cities();

    random_device rd;
    tour tr(t);
    vector<int> indexes;
    vector<city> temp_tour;
    for(int i=1; i<t.get_num_cities(); i++){
        indexes.push_back(i);
    }
    _tours.push_back(t);
    for(int i=1; i<_num_chrom; i++){
        temp_tour.push_back(t[0]);
        mt19937 g(rd());
        shuffle(indexes.begin(), indexes.end(), g);
        for(int j=0; j < t.get_num_cities()-1; j++){
            temp_tour.push_back(t[indexes[j]]);
        }
        tr.set_cities(temp_tour);
        tr.check();
        _tours.push_back(tr);
        temp_tour.clear();
    }
}

int population :: get_num_chrom(){
    return _num_chrom;
}

//prints city index for each tour
void population :: print(){
    for(int i=0; i<_num_chrom; i++){
        for(int j=0; j<_num_cities; j++){
            cout << _tours[i][j].index << " ";
        }
        cout << endl;
    }
}

//returns the shortest tour
tour population :: get_best_tour(){
    int index = 0;
    double L = _tours[0].path_lenght();
    for(int i=1; i<_num_chrom; i++){
        if (_tours[i].path_lenght() < L){
            L = _tours[i].path_lenght();
            index = i;
        }
    }
    return _tours[index];
}

//returns true if the tours are the same (every index is)
bool population :: _is_same(tour t1, tour t2){
    for(int i=0; i<_num_cities; i++){
        //cout << "Comparing city " << i << ": " << t1[i].index << " vs " << t2[i].index << endl;
        if(t1[i].index != t2[i].index) {
        //cout <<t1[i].index<<t2[i].index<<endl;
        //cout << "Difference found at city " << i << ": " << t1[i].index << " != " << t2[i].index << endl;
        return false;
        }
    }
    //cout << "Tours are identical." << endl;
    return true;
}

//fitness selection function
tour population :: _select(){
    float P[_num_chrom];
    double beta = 0.08;
    double total_lenght = 0;
    double L;
    for(int i=0; i<_num_chrom; i++){
        L = _tours[i].path_lenght();
        P[i] = exp(-beta*L); //the higher prob is the one with the shortest lenght
        total_lenght += P[i];
        //cout << "Tour " << i << ", Length: " << L << ", Raw Probability: " << P[i] << endl;
    }
    //normalization
    for(int i=0; i<_num_chrom; i++){
        P[i] /= total_lenght;
        //cout << "Tour " << i << ", Normalized Probability: " << P[i] << endl;
    }
    double s = P[0];
    int i = 0;
    double prob = _rnd.Rannyu();
    //cout << "Randomly generated probability: " << prob << endl;
    while(s < prob){ //up to when the ith chrom prob is as big as the random generated one
        i++;
        s += P[i];
        //cout << "Cumulative Probability at index " << i << ": " << s << endl;
    }
    //cout << "Selected tour index: " << i << endl;
    //cout<< _tours[i][i+1].index <<endl;
    return _tours[i];
    
}

//it works for an even number of chromosomes
//generates a new population from the old one
void population :: step(){
    vector<tour> offspring;

    for(int i=0; i<_num_chrom/2; i++){ //couples chromosomes
        //adds two chrom from current population to offspring
        offspring.push_back(this->_select());
        offspring.push_back(this->_select());
       
        //check if those selected are the same and eventually it chooses another one
        while(this->_is_same(offspring[2*i], offspring[2*i+1])){
            //cout<<"cerca5"<<endl;
            offspring.pop_back();
            //cout<<"cerca6"<<endl;
            offspring.push_back(this->_select());
            //cout<<"cerca7"<<endl;
        }
        //performs crossover and mutation on the new popolation
        if(_rnd.Rannyu() < _p_c) this->_crossover(offspring[2*i], offspring[2*i+1]);
        //cout<<"cerca8"<<endl;
        if(_rnd.Rannyu() < _p_m_1) this->_mutate_1(offspring[2*i]);
        if(_rnd.Rannyu() < _p_m_1) this->_mutate_1(offspring[2*i+1]);
        //cout<<"cerca9"<<endl;
        if(_rnd.Rannyu() < _p_m_2) this->_mutate_2(offspring[2*i]);
        if(_rnd.Rannyu() < _p_m_2) this->_mutate_2(offspring[2*i+1]);
        //cout<<"cerca10"<<endl;
        if(_rnd.Rannyu() < _p_m_3) this->_mutate_3(offspring[2*i]);
        if(_rnd.Rannyu() < _p_m_3) this->_mutate_3(offspring[2*i+1]);
        //cout<<"cerca11"<<endl;
    }
    _tours = offspring; //updates population
}


//switch two cities in a tour
void population :: _mutate_1(tour& t){
    int index_1, index_2;
    //floor converts it to int
    //generates two random index
    do {
        index_1 = floor(_rnd.Rannyu()*(t.get_num_cities()-1)) + 1;
        index_2 = floor(_rnd.Rannyu()*(t.get_num_cities()-1)) + 1;
    } while (index_1 == 0 || index_2 == 0 || index_1 == index_2);

    city c = t[index_1];
    t.set_city(index_1, t[index_2]);
    t.set_city(index_2, c);
    t.check();
}

//switches two segment of a tour
void population :: _mutate_2(tour& t){
    int L_max, L;
    L_max = floor((t.get_num_cities()-1)/2);
    L = floor(_rnd.Rannyu()*(L_max-1)) + 1; 
    int a_start = floor(_rnd.Rannyu()*(t.get_num_cities()-1)); 
    int a_end = floor(_rnd.Rannyu(a_start + L, a_start + t.get_num_cities() - L));
    city c;

    for(int i=0; i<L; i++){
        c = t[(a_start + i)%(t.get_num_cities()-1) + 1];
        t.set_city((a_start + i)%(t.get_num_cities()-1) + 1, t[(a_end + i)%(t.get_num_cities()-1) + 1]);
        t.set_city((a_end + i)%(t.get_num_cities()-1) + 1, c);
    }
    t.check();
}

//inversion of a tour segment
void population :: _mutate_3(tour& t){
    int a_start = floor(_rnd.Rannyu()*(t.get_num_cities()-1));
    int L = floor(_rnd.Rannyu(1, t.get_num_cities()-1));
    city c;

    for(int i=0; i<floor(L/2); i++){
        c = t[(a_start + i)%(t.get_num_cities()-1) + 1];
        t.set_city((a_start + i)%(t.get_num_cities()-1) + 1, t[(a_start + L - 1 - i)%(t.get_num_cities()-1) + 1]);
        t.set_city((a_start + L - 1 - i)%(t.get_num_cities()-1) + 1, c);
    }
    t.check();
}

void population :: _crossover(tour& t1, tour& t2){
    int num_ci = t1.get_num_cities();
    vector<city> copy_t1;
    for(int i=0; i<num_ci; i++){
        copy_t1.push_back(t1[i]);
    }

    int a = floor(_rnd.Rannyu(1, num_ci)); //choose a random starting point
    vector<int> index_t1;
    for(int i=a; i<num_ci; i++){
        for(int j=1; j<num_ci; j++){
            if(t2[j].index == t1[i].index) index_t1.push_back(j);
        }
    }
    sort(index_t1.begin(), index_t1.end());
    for(int i=a; i<num_ci; i++){
        t1.set_city(i, t2[index_t1[i - a]]);
    }
    vector<int> index_t2;
    for(int i=a; i<num_ci; i++){
        for(int j=1; j<num_ci; j++){
            if(copy_t1[j].index == t2[i].index) index_t2.push_back(j);
        }
    }
    sort(index_t2.begin(), index_t2.end());
    for(int i=a; i<num_ci; i++){
        t2.set_city(i, copy_t1[index_t2[i - a]]);
    }
    t1.check();
    t2.check();
}

//average of paths from the best half of tours
double population :: half_AV(){
    vector<double> lenghts_half; //lenght of all tours
    for(int i=0; i<_num_chrom; i++){
        lenghts_half.push_back(_tours[i].path_lenght());
        sort(lenghts_half.begin(), lenghts_half.end()); //lenght in increasing order
    }
    double sum = 0;
    for(int i=0; i<_num_chrom/2; i++){
        sum += lenghts_half[i];
    }
    return sum / (_num_chrom/2);
}

void population::replace_worst_tour(const tour& new_tour) {
    int worst_index = 0;
    double max_length = _tours[0].path_lenght();
    
    for (int i = 1; i < _num_chrom; i++) {
        if (_tours[i].path_lenght() > max_length) {
            max_length = _tours[i].path_lenght();
            worst_index = i;
        }
    }
    _tours[worst_index] = new_tour;
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