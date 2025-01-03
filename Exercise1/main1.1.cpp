#include <iostream>
#include <fstream>
#include <string>
#include "random.h"
#include <cmath>

using namespace std;
 
int main (int argc, char *argv[]){

   //VARIABILI per generare numeri casuali
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



   //VARIABILI
   int M=100000;                      //numeri generati casualmente
   int N=100;                        //numero di blocchi
   int L=int(M/N);                   //numeri in ogni blocco

   double av[N];                 //vettore di N numeri del quale poi farò la media, av sta per average
   double avd[N];                //vettore come sopra ma per la varianza
   double av2[N];                //vettore di N numeri del quale poi farò la media al quadrato
   double av2d[N];               //vettore come sopra ma per la varianza

   double av_prog=0;            //vettore medie progressive 
   double avd_prog=0;
   double av2_prog=0;           //vettore medie quadre progressive
   double av2d_prog=0;
   double err_bar=0;            //vettore barre di errore
   double err_bard=0;

   //creazione di file punto1 per stampare i risultati e poi fare i grafici con Python
   ofstream Graf1;
   Graf1.open("punto12.out");
   if (!Graf1){
        cerr << "Non si riesce ad aprire il file punto1!"<<endl;
        return -1;
   }

   //MEDIE
   for (int i=0; i<N; i++){  //loop sui 100 (N) blocchi          
      double sum1=0;
      double sumd=0;

         for(int j=0; j<L; j++){  //creo le somme di 100 (L) numeri generati casualmente in ogni blocco //dubbio se j oppure k=i+j*L
            sum1 = sum1+rnd.Rannyu();
            sumd = sumd+pow(rnd.Rannyu()-0.5,2);
         }

      av[i]=sum1/double(L); //riempio il vettore delle medie
      avd[i]=sumd/double(L);
      
      av2[i]=pow((av[i]),2); //riempio il vettore dei quadrati delle medie
      av2d[i]=pow((avd[i]),2);
   };        


   //MEDIE PROGRESSIVE e BARRE DI ERRORE
   for (int i=0; i<N; i++){ //ciclo sui 100 (N) blocchi            
      for (int j=0; j<(i+1); j++){ //ciclo sui 100 (L) numeri per blocco
         av_prog+= av[j];            //somme progressive
         avd_prog+= avd[j];

         av2_prog+=av2[j];           //somme progressive dei quadrati
         av2d_prog+=av2d[j];
      };
   
      av_prog/=(i+1);                //medie progressive
      avd_prog/=(i+1);

      av2_prog/=(i+1);               //quadrati delle medie progressive
      av2d_prog/=(i+1);

      err_bar= sqrt((av2_prog-pow(av_prog,2))/(i+1)); 
      err_bard= sqrt((av2d_prog-pow(avd_prog,2))/(i+1));

      Graf1 << i+1 << "\t"<< av_prog<< "\t"<< err_bar << "\t"<< avd_prog<<"\t"<<err_bard<<endl;
     
   };                           
   
   //TEST DEL CHI QUADRO
   double t=0;
   int index=0;
   int nintervalli=100;
   int intervalli[nintervalli];
   double chi[100];

   ofstream Graf3;
   Graf3.open("punto3.out"); //apri un file su cui salvare i risultati
   if(Graf3.is_open()){
      for (int y=0; y<100; y++){ //inizializzo gli intervalli a 0
         for(int j=0; j<nintervalli; j++){
            intervalli[j]=0;
         }
         for(int s=0; s<10000; s++){ //riempio gli intervalli con numeri casuali
            t= rnd.Rannyu();
            index=(t*nintervalli); //calcola l'indice dell'intervallo
            intervalli[index]++;
         }

         double chiquadro=0; //calcola il test del chi quadro
         for(int w=0; w<nintervalli; w++){
            chiquadro+=(pow((intervalli[w]-double(10000)/nintervalli),2))/(double(10000)/nintervalli); //da capire
         }
         chi[y]=chiquadro;
         Graf3 << y+1 <<"\t"<<chi[y]<<endl; //salva il valore del chi quadro sul file " .out"
      };

   } else cerr <<"PROBLEM: Unable to open punto3.out"<<endl;

   
   Graf1.close();
   Graf3.close();

   return 0;
}