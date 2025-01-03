#include <iostream>
#include <fstream>
#include <string>
#include "random.h"
#include <cmath>
#include <vector>

using namespace std;
 
int main (int argc, char *argv[]){

   Random rnd;                      //VARIABILI
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


   //rnd.SaveSeed(); perchè l'ho scritto in primo luogo?

   //VARIABILI
   int M=10000; //numero di throws
   int N=100; //numero di blocchi
   int L=int(M/N); //throws per blocco


   double av[N]; //vettore di N numeri del quale poi faro la media, av sta per average
   double avd[N]; //vettore come sopra ma per la varianza
   double av2[N]; //vettore di N numeri del quale poi farò la media al quadrato
   double av2d[N]; //vettore come sopra ma per la varianza

   double av_prog[N]; //vettore medie progressive 
   double avd_prog[N];
   double av2_prog[N]; //vettore medie quadre progressive
   double av2d_prog[N];
   double err_bar[N]; //vettore barre di errore
   double err_bard[N];




   //DISTRIBUZIONE UNIFORME
   //creazione di file integralUnif per stampare i risultati e poi fare i grafici con Python
   ofstream Graf1;
   Graf1.open("integralUnif.out");
   if (!Graf1){
        cerr << "Non si riesce ad aprire il file integralUnif!"<<endl;
        return -1;
   }
   //MEDIE
   for (int i=0; i<N; i++){  //loop sui 100 (N) blocchi          
      double sum1=0; //contatore dei valori della funzione
      double x; //ascissa

         for(int j=0; j<L; j++){  //creo le somme di 100 (L) numeri generati casualmente in ogni blocco //dubbio se j oppure k=i+j*L
            x=rnd.Rannyu(); //genero un'ascissa tra 0 e 1
            sum1+=M_PI/2*(cos(x*M_PI/2)); //aumento l'area andando a sommare i valori della funzione calcolata nel numero estratto unif tra 0 e 1  
         };

      av[i]=sum1/L; //riempio il vettore delle medie con i vari integrali
      av2[i]=pow((av[i]),2); //riempio il vettore dei quadrati delle medie
   }
   //MEDIE PROGRESSIVE e BARRE DI ERRORE
   for (int i=0; i<N; i++){ //ciclo sui 100 (N) blocchi            
      for (int j=0; j<i+1; j++){ //ciclo sui 100 (L) numeri per blocco
         av_prog[i]+= av[j];            //somme progressive
         av2_prog[i]+=av2[j];           //somme progressive dei quadrati
      };
   
      av_prog[i]/=(1+i);                //medie progressive
      av2_prog[i]/=(1+i);               //quadrati delle medie progressive
      
      if (i>0){
         err_bar[i]= sqrt((av2_prog[i]-pow(av_prog[i],2))/i);
         } else {err_bar[i]=0;}


      Graf1 << i+1 <<"\t"<< av_prog[i]<<"\t"<< err_bar[i] <<endl; //stampa su file l'indice di blocco e le medie progressive con gli errori corrispondenti
     
   }                           

   

   //DISTRIBUZIONE CON IMPORTANCE SAMPLING
   //creazione di file integralSamp per stampare i risultati e poi fare i grafici con Python
   ofstream Graf2;
   Graf2.open("integralSamp.out");
   if (!Graf2){
        cerr << "Non si riesce ad aprire il file integralSamp!"<<endl;
        return -1;
   }
   //MEDIE
   for (int i=0; i<N; i++){  //loop sui 100 (N) blocchi          
      double sum1=0; //contatore dei valori della funzione
         for(int j=0; j<L; j++){  //creo le somme di 100 (L) numeri generati casualmente in ogni blocco //dubbio se j oppure k=i+j*L
            double y = rnd.Rannyu();
            double x = 1 - sqrt(1 - y);  // Inversione della cumulativa
            double fx = M_PI/2 * cos(M_PI*x/2);
            double px = (2 - 2*x);  // Funzione di densità di probabilità
            sum1 += fx / px;
         }

      avd[i]=sum1/L; //riempio il vettore delle medie con i vari integrali
      av2d[i]=pow((avd[i]),2); //riempio il vettore dei quadrati delle medie
   };
   //MEDIE PROGRESSIVE e BARRE DI ERRORE
   for (int i=0; i<N; i++){ //ciclo sui 100 (N) blocchi            
      for (int j=0; j<i+1; j++){ //ciclo sui 100 (L) numeri per blocco
         avd_prog[i]+= avd[j];            //somme progressive
         av2d_prog[i]+=av2d[j];           //somme progressive dei quadrati
      }
      avd_prog[i]/=(1+i);                //medie progressive
      av2d_prog[i]/=(1+i);               //quadrati delle medie progressive
      if (i>0){
         err_bard[i]= sqrt((av2d_prog[i]-pow(avd_prog[i],2))/i);
         } else {err_bard[i]=0;}
      
      Graf2 << i+1 <<"\t"<< avd_prog[i]<<"\t"<< err_bard[i] <<endl; //stampa su file l'indice di blocco e le medie progressive con gli errori corrispondenti
   }


   //esercizio 2.2
   const double a = 1.0; // Lunghezza del passo
   const int steps = 100; // Numero di passi per RW

    // Variabili per Random Walk discreta e continua
    vector<double> r2_dis(steps, 0.0), r2_dis2(steps, 0.0);
    vector<double> r2_con(steps, 0.0), r2_con2(steps, 0.0);

    // Simulazioni per blocchi
    for (int i = 0; i < N; i++) {
        vector<double> r2_dis_block(steps, 0.0);
        vector<double> r2_con_block(steps, 0.0);

        for (int j = 0; j < L; j++) {
            // Partenza dall'origine
            double x_dis = 0.0, y_dis = 0.0, z_dis = 0.0;
            double x_con = 0.0, y_con = 0.0, z_con = 0.0;

            for (int step = 0; step < steps; step++) {
                // Random Walk discreta
                int direction = int(rnd.Rannyu(0, 3));
                int step_dis = (rnd.Rannyu() < 0.5) ? 1 : -1;
                if (direction == 0) x_dis += step_dis * a;
                else if (direction == 1) y_dis += step_dis * a;
                else z_dis += step_dis * a;

                // Random Walk continua
                double theta = acos(1 - 2 * rnd.Rannyu());
                double phi = rnd.Rannyu(0, 2 * M_PI);
                x_con += a * sin(theta) * cos(phi);
                y_con += a * sin(theta) * sin(phi);
                z_con += a * cos(theta);

                // Aggiorna le distanze quadrate
                r2_dis_block[step] += x_dis * x_dis + y_dis * y_dis + z_dis * z_dis;
                r2_con_block[step] += x_con * x_con + y_con * y_con + z_con * z_con;
            }
        }

        // Media dei blocchi
        for (int step = 0; step < steps; step++) {
            r2_dis_block[step] /= L;
            r2_con_block[step] /= L;

            // Accumula i risultati globali
            r2_dis[step] += r2_dis_block[step];
            r2_dis2[step] += r2_dis_block[step] * r2_dis_block[step];
            r2_con[step] += r2_con_block[step];
            r2_con2[step] += r2_con_block[step] * r2_con_block[step];
        }
    }

    // Finalizza le medie
    vector<double> r_dis(steps, 0.0), r_con(steps, 0.0);
    vector<double> err_dis(steps, 0.0), err_con(steps, 0.0);
    for (int step = 0; step < steps; step++) {
        r2_dis[step] /= N;
        r2_dis2[step] /= N;
        r2_con[step] /= N;
        r2_con2[step] /= N;

        r_dis[step] = sqrt(r2_dis[step]);
        r_con[step] = sqrt(r2_con[step]);

        err_dis[step] = sqrt((r2_dis2[step] - r2_dis[step] * r2_dis[step]) / step);
        err_con[step] = sqrt((r2_con2[step] - r2_con[step] * r2_con[step]) / step);
    }

    // Salva i risultati nei file
    ofstream Graf3("RandomwalkD.out");
    ofstream Graf4("RandomwalkC.out");
    if (!Graf3 || !Graf4) {
        cerr << "Errore nell'apertura dei file di output!" << endl;
        return -1;
    }

    for (int step = 0; step < steps; step++) {
        Graf3 << step + 1 << "\t" << r_dis[step] << "\t" << err_dis[step] << endl;
        Graf4 << step + 1 << "\t" << r_con[step] << "\t" << err_con[step] << endl;
    }

     
   
   Graf1.close();
   Graf2.close();
   Graf3.close();
   Graf4.close();

   return 0;
}