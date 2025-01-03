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

//creazione di file buffon per stampare i risultati e poi fare i grafici con Python
ofstream Graf4;
Graf4.open("buffon.out");
if (!Graf4){
      cerr << "Non si riesce ad aprire il file buffon!"<<endl;
      return -1;
}

//variabili
double L = 1;  // Lunghezza ago
double d = 3;  // Distanza tra le linee (assicurati che d > L)
int M = 100000;  // Numero totale di lanci
int N = 100;  // Numero di blocchi
//int S = int (M / N); //throws for each block (risultano troppo pochi per approssimare bene pi)
int nhit=0; //numero di volte in cui l'estrazione cade nella zona considerata
double xp=0; //numero casuale nella metà superiore dello spazio
double a=0; //coordinates
double b=0; //coordinates
double m=0;
double pi[100]; //array per salvare le stime di pi
double sum = 0, sum2 = 0; //vettori per le dev standard

for(int i=0; i<N; i++){
   nhit=0; //reset the counting
   for(int j=0; j<100000; j++){
      xp=rnd.Rannyu(d/2,d);
      if(rnd.Rannyu()<L/2){
         do {
            a=rnd.Rannyu(-L,L);
            b=rnd.Rannyu(-L,L);
         } while(sqrt(pow(a,2)+pow(b,2))>1 || a==0);
         m=b/a;
         if((xp+sqrt(pow(m,2)/(1+pow(m,2))))>d){
            nhit++;
         }
      }
   }
   pi[i]=(2*L*100000)/(nhit*d);

  // Aggiorna le somme per il calcolo della deviazione standard
   sum += pi[i];
   sum2 += pi[i] * pi[i];
    
    // Calcola la media e la deviazione standard fino a questo punto
   double mean = sum / (i+1);
   double variance = (sum2 / (i+1)) - (mean * mean);
   double std_dev;
   if (i > 0) {
      std_dev = sqrt(variance / i);
   } else {
      std_dev = 0;
   }
    
    // Stampa i risultati per questo blocco
    Graf4 << i+1 << "\t" << mean << "\t" << std_dev << endl;

};

Graf4.close();

 return 0;
}