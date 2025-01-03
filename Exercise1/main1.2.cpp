#include <iostream>
#include <fstream>
#include <string>
#include "random.h"
#include <cmath>

using namespace std;
 
int main (int argc, char *argv[]){

   Random rnd;                      
   int seed[4];
   int p1, p2;

   ifstream Primes("Primes"); //apri il file Primes e salvaci dentro due interi che serviranno poi a .SetRandom
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in"); //apri il file seed.in e una volta che hai letto la string RANDOMSEED salva nel vettore seed i 4 numeri che legg subito dopo e poi usa .SetRandom per inizializzare i parametri della funzione random
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


   rnd.SaveSeed();

   int M=10000; //numero di estrazioni su cui si vuole verificare il limite centrale

   //distribuzione uniforme
   ofstream Graf2Unif; //scrivi su un file " .out" le varie estrazioni con diversi N
   Graf2Unif.open("punto12Unif.out");
   double k=0;
   for(int i=0; i<M; i++){ //ciclo for su 10000 estrazioni per verificare il limite centrale
      k=0;
      for(int j=0; j<10; j++){ //ciclo for per verificare la media di 10 estrazioni (N=10)
         k=k+rnd.Rannyu();
      }
      Graf2Unif<<rnd.Rannyu()<<"\t"<<(rnd.Rannyu()+rnd.Rannyu())/2<<"\t"<<k/10<<"\t"; //riempi il file " .out" con una sola estrazzione N=1 e le medie delle estrazioni per N=2 ed N=10
      k=0;
      for(int j=0; j<100; j++){ //ciclo for per fare le medie di 100 estrazioni (N=100)
         k+=rnd.Rannyu();
      }
      Graf2Unif<<k/100<<endl;
   }

   //distribuzione esponenziale
   ofstream Graf2Exp; //scrivi su un file " .out" le varie estrazioni con diversi N
   Graf2Exp.open("punto12Exp.out");
   double lambda=1;
   for(int i=0; i<M; i++){ //ciclo for su 10000 estrazioni per verificare il limite centrale
      k=0;
      for(int j=0; j<10; j++){ //ciclo for per verificare la media di 10 estrazioni (N=10)
         k+=rnd.Exponential(lambda);
      }
      Graf2Exp<<rnd.Exponential(lambda)<<"\t"<<(rnd.Exponential(lambda)+rnd.Exponential(lambda))/2<<"\t"<<k/10<<"\t"; //riempi il file " .out" con una sola estrazzione N=1 e le medie delle estrazioni per N=2 ed N=10
      k=0;
      for(int j=0; j<100; j++){ //ciclo for per fare le medie di 100 estrazioni (N=100)
         k+=rnd.Exponential(lambda);
      }
      Graf2Exp<<k/100<<endl;
   }   

   //distribuzione Cauchy-Lorentz
   ofstream Graf2Lor; //scrivi su un file " .out" le varie estrazioni con diversi N
   Graf2Lor.open("punto12Lor.out"); 
   double gamma=1;
   double mu=0;
   for(int i=0; i<M; i++){ //ciclo for su 10000 estrazioni per verificare il limite centrale
      k=0;
      for(int j=0; j<10; j++){ //ciclo for per verificare la media di 10 estrazioni (N=10)
         k+=rnd.Lorentzian(mu, gamma);
      }
      Graf2Lor<<rnd.Lorentzian(mu, gamma)<<"\t"<<(rnd.Lorentzian(mu, gamma)+rnd.Lorentzian(mu, gamma))/2<<"\t"<<k/10<<"\t"; //riempi il file " .out" con una sola estrazzione N=1 e le medie delle estrazioni per N=2 ed N=10
      k=0;
      for(int j=0; j<100; j++){ //ciclo for per fare le medie di 100 estrazioni (N=100)
         k+=rnd.Lorentzian(mu, gamma);
      }
      Graf2Lor<<k/100<<endl;
   }  

   //chiudo tutti i file aperti
   Graf2Unif.close();
   Graf2Exp.close();
   Graf2Lor.close();

   return 0;
}