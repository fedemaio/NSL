#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <utility>
#include <sstream>
#include "random.h"

using namespace std;

//trial wave function
double psi(double x, double mu, double sigma){
    return exp(-pow((x - mu), 2) / (2 * pow(sigma, 2))) + exp(-pow((x + mu), 2) / (2 * pow(sigma, 2)));
}

//hamiltonian
//kinetic energy as second derivative + potential energy
double hamiltonian(double x, double mu, double sigma){
    return (((pow(x+mu,2)*exp(-pow(x+mu,2)/(2*pow(sigma,2))))/pow(sigma,4) - exp(-pow(x+mu,2)/(2*pow(sigma,2)))/pow(sigma,2)
           + (pow(x-mu,2)*exp(-pow(x-mu,2)/(2*pow(sigma,2))))/pow(sigma,4) - exp(-pow(x-mu,2)/(2*pow(sigma,2)))/pow(sigma,2))*(-0.5))/psi(x, mu, sigma)
           + pow(x,4) - pow(x,2)*5.0/2;
}

//Metropolis step to generate x
//it randomly generates, calculates psi^2 with either old and new x, accepts if prob new > prob old
//returns step accepted or rejected
bool step(double &x, Random &rnd, double sigma, double mu, int ntot, int nacc, double delta){
    ntot ++;
    double x_new = rnd.Rannyu(x - delta, x + delta);
    double prob_old = pow(psi(x, mu, sigma), 2);
    double prob_new = pow(psi(x_new, mu, sigma), 2);
    if(prob_new > prob_old){
        x = x_new;
        nacc ++;
        return true;
    } else {
        if(rnd.Rannyu() < prob_new/prob_old) {
        x = x_new;
        nacc ++;
        return true;
        } else return false;
    }
}

//
pair<double, double> calculate_hamiltonian(double mu, double sigma, Random &rnd, 
    int ntot, int nacc, double delta, int N, int m, double* AV, 
    double* av_prog, double* av2_prog, double* err_bar) {
    
    //hamiltonian
    double x = rnd.Rannyu(-1, 1);
    double current;
    for(int i = 0; i < N; i++) {
        current = 0;
        for(int j = 0; j < m; j++) {
            current += hamiltonian(x, mu, sigma);
            step(x, rnd, sigma, mu, ntot, nacc, delta);
        }
        AV[i] = current/m;   
    }
    //medie progressive e degli errori
    for (int i = 0; i < N; i++) {
        av_prog[i] = 0;    
        av2_prog[i] = 0;   
        
        for (int j = 0; j <= i; j++) {
            av_prog[i] += AV[j];
            av2_prog[i] += pow(AV[j], 2);
        }
        
        av_prog[i] /= (i + 1);
        av2_prog[i] /= (i + 1);
        
        if (i > 0) {
            err_bar[i] = sqrt((av2_prog[i] - pow(av_prog[i], 2))/i);
        } else {
            err_bar[i] = 0;
        }
    }
    return {av_prog[N-1], err_bar[N-1]};
}


void calculate_hamilton_file(double mu, double sigma, Random &rnd, 
    int ntot, int nacc, double delta, int N, int m, double* AV, 
    double* av_prog, double* av2_prog, double* err_bar, 
    std::ofstream& file, std::ofstream& file_position) {
    
    // Calcolo dei valori dell'hamiltoniana
    double x = rnd.Rannyu(-1, 1);
    file_position << x << std::endl;  // Scrivi la posizione iniziale
    
    double current;
    for(int i = 0; i < N; i++) {
        current = 0;
        for(int j = 0; j < m; j++) {
            current += hamiltonian(x, mu, sigma);
            step(x, rnd, sigma, mu, ntot, nacc, delta);
            file_position << x << endl;  // Scrivi ogni nuova posizione
        }
        AV[i] = current/m;   
    }
    //medie progressive e degli errori
    for (int i = 0; i < N; i++) {
        av_prog[i] = 0;   
        av2_prog[i] = 0;   
        
        for (int j = 0; j <= i; j++) {
            av_prog[i] += AV[j];
            av2_prog[i] += pow(AV[j], 2);
        }
        
        av_prog[i] /= (i + 1);
        av2_prog[i] /= (i + 1);
        
        if (i > 0) {
            err_bar[i] = sqrt((av2_prog[i] - pow(av_prog[i], 2))/i);
        } else {
            err_bar[i] = 0;
        }
        file << i+1 << "\t" << av_prog[i] << "\t" << err_bar[i] << endl;
    }
}