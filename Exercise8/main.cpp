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
#include <vector>
#include "random.h"
#include "groundstate_SA.h"

using namespace std;
 
int main (int argc, char *argv[]){

   ////////////////////////////////////////////////////////////////////////set random objects/////////////////////////////////////////////////////////////////////////////////////////////////
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


   ////////////////////////////////////////////////////////////////////////////variables//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //block averages
   int M=100; //numero di throws
   const int N=100; //numero di blocchi

   //Hamiltonian parameters
   double mu=1; 
   double sigma=1;
   double mu_new, sigma_new;
   //double H;
   //double H_err;
   //double x=rnd.Rannyu();

   //Metropolis algorithm
   double delta_T=1; //Metropolis acceptance interval
   double delta_par=0.1; //delta for mu and sigma
   double T=1.0; //initial temperature for SA
   int ntot=0;
   int nacc=0;
   double acceptance_rate_T;
   int nacc_T; //acceptance for decreasing of temperature
   int step_SA = 0;
   double* AV=new double[N];
   double* av_prog=new double[N];
   double* av2_prog=new double[N];
   double* err_barr=new double[N];
   double beta;
   int m_plus = 10000;
   double prob;

   //files
   //file to save sigma and mu variations
   ofstream out_parameters;
   out_parameters.open("Parameters.out");
   //file to save H as functions of SA steps
   ofstream out_H_SAsteps;
   out_H_SAsteps.open("Hamiltonian_SA.out");
   //file to save H as functions of blocks
   ofstream out_H_blocksteps;
   out_H_blocksteps.open("Hamiltonian.out");
   //file to print positions
   ofstream out_x;
   out_x.open("Positions.out");

   
   ////////////////////////////////////////////////////////////////////////exercise 8///////////////////////////////////////////////////////////////////////////////////////
   //initial hamiltonian, with fixed parameters
   auto[H, H_err]=calculate_hamiltonian(mu, sigma, rnd, ntot, nacc, delta_T, N, M, AV, av_prog, av2_prog, err_barr);
   out_parameters << mu << "\t" << sigma << endl;

   //cout <<H<<"\t"<<H_err<<endl;

   //Simulated annealing cycle as temperature reaches 0.001
   while (T > 0.001){
        nacc_T = 0;
        beta = 1/T;
      
        for(int k=0; k<100; k++){
            sigma_new = sigma + delta_par * (rnd.Rannyu()*2-1); //perchè -1?
            mu_new = mu + delta_par * (rnd.Rannyu()*2-1);
            auto[H_new, H_new_err] = calculate_hamiltonian(mu_new, sigma_new, rnd, ntot, nacc, delta_T, N, M, AV, av_prog, av2_prog, err_barr);
            prob = exp(-beta*(H_new - H));

            if (H_new < H || rnd.Rannyu() < prob){
                sigma = sigma_new;
                mu = mu_new;
                H = H_new;
                H_err = H_new_err;
                nacc_T ++;
                out_parameters << mu << "\t" << sigma << endl;
            }
        }
        out_H_SAsteps << step_SA + 1 << "\t" << H << "\t" << H_err << endl;
        acceptance_rate_T = (double)nacc_T/100;
        if (acceptance_rate_T < 0.3){
            delta_par *= 0.81;
        } else if(acceptance_rate_T > 0.7){
            delta_par *= 1.21;
        }
        //cout << "acceptance for T = " << T << ": " << acceptance_rate_T << endl;
        T *= 0.95;  
        step_SA ++;
        //cout << step_SA<<endl;
   }

   calculate_hamilton_file(mu, sigma, rnd, ntot, nacc, delta_T, N, m_plus, AV, av_prog, av2_prog, err_barr, out_H_blocksteps, out_x);

   //////////////////////////////////////////////////////////print of mu, sigma, ground state and its error////////////////////////////////////////////////////////////////////////////////////
   cout << "SIGMA: " << sigma << endl;
   cout << "MU: " << mu << endl;
   cout << "GROUND STATE ENERGY: " << H << "    WITH AN ERROR: " << H_err << endl;

   out_x.close();
   out_parameters.close();
   out_H_SAsteps.close();
   out_H_blocksteps.close();

   rnd.SaveSeed();
   return 0;
}