/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/

#include <string>
#include <cmath>
#include "population.h"

using namespace std;
 
int main (int argc, char *argv[]){

   Random rnd;
   int seed[4];
   int p1, p2;
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;

   ///////////////////////////////////////////////////////////////////////////exercise 9///////////////////////////////////////////////////////////////////////////////////////////////
   
   //read properties from input file
   int N_cities;
   int steps;
   int N; //number of chromosomes
   double p_crossover;
   double p_mutate1, p_mutate2, p_mutate3;

   ifstream inputParams("input.txt");
   if (!inputParams.is_open()) {
      cerr << "PROBLEM: Unable to open input.txt" << endl;
      return -1;
   }
   string paramName;
   double paramValue;
   while (inputParams >> paramName >> paramValue) {
      if (paramName == "N_cities") N_cities = (int)paramValue;
      else if (paramName == "steps") steps = (int)paramValue;
      else if (paramName == "N") N = (int)paramValue;
      else if (paramName == "p_crossover") p_crossover = paramValue;
      else if (paramName == "p_mutate1") p_mutate1 = paramValue;
      else if (paramName == "p_mutate2") p_mutate2 = paramValue;
      else if (paramName == "p_mutate3") p_mutate3 = paramValue;
   }
   inputParams.close();

   cout<< N_cities << steps << N << p_crossover << p_mutate1 << p_mutate2 << p_mutate3 <<endl;
   
   ////////////////////////////////////////////////////////////////////////////variables and files///////////////////////////////////////////////////////////////////////////////////////////////////////
   vector<city> initial_cities_C;
   double angle;
   vector<city> initial_cities_Q;
   double x,y;

   ofstream out_average_pathC;
   out_average_pathC.open("averages_C.out");
   ofstream out_positions_C;
   out_positions_C.open("coordinates_C.out");
   ofstream out_average_pathQ;
   out_average_pathQ.open("averages_Q.out");
   ofstream out_positions_Q;
   out_positions_Q.open("coordinates_Q.out");

   ////////////////////////////////////////////////////////////////////////////circumference////////////////////////////////////////////////////////////////////////////////////////////////////
   //initialize cities for circolar path
   for(int i=0; i<N_cities; i++){
      angle = rnd.Rannyu(0, 2*M_PI);
      city c(i, cos(angle), sin(angle));
      initial_cities_C.push_back(c);
   }

   //initialize circolar path
   tour initial_tour(initial_cities_C);
   population pop(rnd, N, initial_tour, p_crossover, p_mutate1, p_mutate2, p_mutate3);
   
   for(int i=0; i<steps; i++){
      pop.step();
      out_average_pathC << i+1 << "\t" << pop.get_best_tour().path_lenght() << "\t" << pop.half_AV() << endl;
   }
   
   //perform GA 
   for(int i=0; i<N_cities; i++){
      out_positions_C << pop.get_best_tour()[i].index << "\t" << pop.get_best_tour()[i].x << "\t" << pop.get_best_tour()[i].y << endl;
   }
   out_positions_C << pop.get_best_tour()[0].index << "\t" << pop.get_best_tour()[0].x << "\t" << pop.get_best_tour()[0].y << endl; 
   

   ///////////////////////////////////////////////////////////////////////////square///////////////////////////////////////////////////////////////////////////////////////
   //initialize cities for circolar path
   for(int i=0; i<N_cities; i++){
      x = rnd.Rannyu(0, 1);
      y = rnd.Rannyu(0, 1);
      city c(i, x, y);
      initial_cities_Q.push_back(c);
   }

   //initialize square path
   tour initial_tour_Q(initial_cities_Q);
   population pop_Q(rnd, N, initial_tour_Q, p_crossover, p_mutate1, p_mutate2, p_mutate3);

   for(int i=0; i<steps; i++){
      pop_Q.step();
      out_average_pathQ << i+1 << "\t" << pop_Q.get_best_tour().path_lenght() << "\t" << pop_Q.half_AV() << endl;
   }
   
   //perform GA
   for(int i=0; i<N_cities; i++){
      out_positions_Q << pop_Q.get_best_tour()[i].index << "\t" << pop_Q.get_best_tour()[i].x << "\t" << pop_Q.get_best_tour()[i].y << endl;
   }
   out_positions_Q << pop_Q.get_best_tour()[0].index << "\t" << pop_Q.get_best_tour()[0].x << "\t" << pop_Q.get_best_tour()[0].y << endl; 
   

   out_average_pathC.close();
   out_positions_C.close();
   out_average_pathQ.close();
   out_positions_Q.close();
   
   rnd.SaveSeed();
   return 0;
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************/