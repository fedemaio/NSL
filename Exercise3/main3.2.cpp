#include <iostream>
#include <fstream>
#include <string>
#include "random.h"
#include <cmath>

using namespace std;

double mass(double a, double b)
{
   if (a > b)
      return a;
   return b;
}

int main(int argc, char *argv[])
{

   Random rnd; // VARIABILI
   int seed[4];
   int p1, p2;

   ifstream Primes("Primes"); // apri il file Primes e salvaci dentro due interi che serviranno poi a .SetRandom
   if (Primes.is_open())
   {
      Primes >> p1 >> p2;
   }
   else
      cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in"); // apri il file seed.in e una volta che hai letto la string RANDOMSEED salva nel vettore seed i 4 numeri che legg subito dopo e poi usa .SetRandom per inizializzare i parametri della funzione random
   string property;
   if (input.is_open())
   {
      while (!input.eof())
      {
         input >> property;
         if (property == "RANDOMSEED")
         {
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed, p1, p2);
         }
      }
      input.close();
   }
   else
      cerr << "PROBLEM: Unable to open seed.in" << endl;

   rnd.SaveSeed();

   // VARIABILI
   int M = 10000;      // numeri generati casualmente
   int N = 100;        // numero di blocchi
   int L = int(M / N); // numeri in ogni blocco

   double av[N];   // vettore di N numeri del quale poi faro la media, av sta per average
   double avd[N];  // vettore come sopra ma per la varianza
   double av2[N];  // vettore di N numeri del quale poi faro la media al quadrato
   double av2d[N]; // vettore come sopra ma per la varianza

   double avc_prog[N]; // vettore medie progressive
   double avp_prog[N];
   double av2_prog[N]; // vettore medie quadre progressive
   double av2d_prog[N];
   double errc_barr[N]; // vettore barre di errore
   double errp_barr[N];

   double So = 100;     // prezzo al tempo iniziale del bene
   double T = 1;        // tempo al quale termina il contratto
   double K = 100;      // prezzo stabilito
   double r = 0.1;      // tasso di interesse
   double sigma = 0.25; // volatilità

   double Call = 0;
   double Put = 0;

   // creazione di file punto1 per stampare i risultati e poi fare i grafici con Python
  
   ofstream Graf3;
   Graf3.open("CallDiscrete.out");
   if (!Graf3)
   {
      cerr << "Non si riesce ad aprire il file CallDiscrete!" << endl;
      return -1;
   }

   ofstream Graf4;
   Graf4.open("PutDiscrete.out");
   if (!Graf4)
   {
      cerr << "Non si riesce ad aprire il file PutDiscrete!" << endl;
      return -1;
   }

   //CALL-Option e PUT-Option discretized
   for (int i=0; i<N; i++){
      Call=0;
      Put=0;

     for (int j = 0; j < L; j++){
        double S = So;
        double dt = T/100.0; // 100 intervalli di tempo

      for (int k = 0; k < 100; k++){
         double dW = rnd.Gauss(0, sqrt(dt));
         S *= exp((r - 0.5*pow(sigma,2))*dt + sigma*dW);
      }

      Call += exp(-r * T) * mass(0, S - K);
      Put += exp(-r * T) * mass(0, K - S);
   }

   av[i] = Call/L;
   avd[i] = Put/L;

   av2[i] = pow((av[i]), 2);
   av2d[i] = pow((avd[i]), 2);
}
     

   // MEDIE PROGRESSIVE e BARRE DI ERRORE
   for (int i = 0; i < N; i++) {

      for (int j = 0; j < i + 1; j++){

         avc_prog[i] += av[j]; // somme progressive
         avp_prog[i] += avd[j];

         av2_prog[i] += av2[j]; // somme progressive dei quadrati
         av2d_prog[i] += av2d[j];
      };

      avc_prog[i] /= (1+i); // medie progressive
      avp_prog[i] /= (1+i);

      av2_prog[i] /= (1+i); // quadrati delle medie progressive
      av2d_prog[i] /= (1+i);

      errc_barr[i] = sqrt((av2_prog[i] - pow(avc_prog[i], 2)) / i);
      errp_barr[i] = sqrt((av2d_prog[i] - pow(avp_prog[i], 2) - (1 / 12)) / i);

      if (i == 0){ 
         Graf3 << 1 << "\t" << avc_prog[i] << "\t" << 0 << endl;
         Graf4 << 1 << "\t" << avp_prog[i] << "\t" << 0 << endl;
      }
      else{
         Graf3 << i+1 << "\t" << avc_prog[i] << "\t" << errc_barr[i] << endl;
         Graf4 << i+1 << "\t" << avp_prog[i] << "\t" << errp_barr[i] << endl;
      }
   };

   Graf3.close();
   Graf4.close();

   return 0;
}