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
#include <mpi.h> 
#include "population.h"

using namespace std;

struct PathLengthWithRank {
    double length;
    int rank;
};
 
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
   

   /////////////////////////////////////////////////////////////////////////////////////exercise 10/////////////////////////////////////////////////////////////////////////////////////////////

   /////////////////////////////////////////////////////////////////////////////////variables and files///////////////////////////////////////////////////////////////////////////////////////////////////////
   MPI_Init(&argc, &argv);
   int size, rank;
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    
   int N_provinces=0;
   int steps=0;
   int N=0; //number of chromosomes
   int N_migr=0; //
   double p_crossover=0;
   double p_mutate1=0, p_mutate2=0, p_mutate3=0;
   bool communicate = false;

   //////////////////////////////////////////////////////////////////////////read properties from input file//////////////////////////////////////////////////////////////////////////
   ifstream inputParams("input.txt");
   if (!inputParams.is_open()) {
      cerr << "PROBLEM: Unable to open input.txt" << endl;
      return -1;
   }

   string paramName;
   double paramValue;
   //bool communicate = false; 

   while (inputParams >> paramName) {
      if (paramName == "communicate") {
         string strValue;
         inputParams >> strValue; 
         if (strValue == "TRUE") communicate = true;
         else if (strValue == "FALSE") communicate = false;
         else {
            cerr << "PROBLEM: Invalid value for 'communicate'" << endl;
            return -1;
         }
      } else {
         inputParams >> paramValue;
         if (paramName == "N_provinces") N_provinces = (int)paramValue;
         else if (paramName == "steps") steps = (int)paramValue;
         else if (paramName == "N") N = (int)paramValue;
         else if (paramName == "N_migr") N_migr = (int)paramValue;
         else if (paramName == "p_crossover") p_crossover = paramValue;
         else if (paramName == "p_mutate1") p_mutate1 = paramValue;
         else if (paramName == "p_mutate2") p_mutate2 = paramValue;
         else if (paramName == "p_mutate3") p_mutate3 = paramValue;
      }
   }
   inputParams.close();
   
   //////////////////////////////////////////////////////////////////initial positions of cities from file////////////////////////////////////////////////////////////////////////////////////////
   vector<city> initial_cities;
   ifstream in;
   double x, y;
   int index;
   in.open("cap_prov_ita.dat");
   for(int i=0; i<N_provinces; i++){
      in >> x >> y;
      index = i;
      city c(index, x, y);
      initial_cities.push_back(c);
   }
   in.close();

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   tour initial_tour(initial_cities);//siccome quando inizializzo pop uso il metodo shuffle per generare i tour e shuffle
   population pop(rnd, N, initial_tour, p_crossover, p_mutate1, p_mutate2, p_mutate3);//genera ogni volta un tour diverso non devo avere seed diversi per i diversi process
   
   for(int i=0; i<steps; i++){
      
      pop.step();
   
      if((i+1)%N_migr == 0 && communicate){ //each N_migr step exchange of best paths 
         
         vector<int> best_trip_int_send = pop.get_best_tour().serialize(); //serialize best tour
         vector<int> best_trip_int_receive(best_trip_int_send.size()); //empty receiver
         //sets ranks
         int partner = rnd.Rannyu(0, size); // Nodo partner casuale
         //int send_to = (rank + 1) % size;
         //int receive_from = (rank - 1 + size) % size;

         int send_size = (int)best_trip_int_send.size();
         int receive_size = (int)best_trip_int_receive.size();

         //exchange of tours in between processors
         MPI_Sendrecv(best_trip_int_send.data(), send_size, MPI_INT, partner, 0, 
                      best_trip_int_receive.data(), receive_size, MPI_INT, partner, 0,
                      MPI_COMM_WORLD, MPI_STATUS_IGNORE);

         //updates best tour
         //pop.get_best_tour().deserialize(best_trip_int_receive);
         tour received_tour;
         received_tour.deserialize(best_trip_int_receive);

         // Sostituisci il peggior tour con il tour ricevuto
         pop.replace_worst_tour(received_tour);
       
      }
      //print check of the state of progress
      if(rank==0) cout << (double)i/steps << endl;
   }

   //each part calculates its rank and its best lenght
   PathLengthWithRank my_path_length_with_rank;
   my_path_length_with_rank.length = pop.get_best_tour().path_lenght();
   my_path_length_with_rank.rank = rank;

   //search for shortest lenght and its rank
   PathLengthWithRank best_path_length_with_rank;
   MPI_Reduce(&my_path_length_with_rank, &best_path_length_with_rank, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
   //prints best path coordinates on file
   cout<<"daje"<<endl;
   if (rank == best_path_length_with_rank.rank) {
      cout << "Rank " << rank << " is writing to " << endl;
      ofstream out_positions;
      string filename = "coordinates_8_TRUE.out";
      out_positions.open(filename);

      if (out_positions.is_open()) {
         for (int i = 0; i < N_provinces; i++) {
               out_positions << pop.get_best_tour()[i].index << "\t"<< pop.get_best_tour()[i].x << "\t" << pop.get_best_tour()[i].y << endl;
         }
         //first city in order to close the path
         out_positions << pop.get_best_tour()[0].index << "\t" << pop.get_best_tour()[0].x << "\t" << pop.get_best_tour()[0].y << endl; 
         //out_positions << best_path_length_with_rank.length << "    000   000" << endl;
         out_positions.close();
      } else {
         cerr << "PROBLEM: Unable to open " << filename << " for writing" << endl;
      }
   }
   //cout<<"ciao6"<<endl;
   MPI_Finalize();
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