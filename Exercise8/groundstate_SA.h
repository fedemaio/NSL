#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <utility>
#include <sstream>
#include "random.h"

using namespace std;

double psi(double x, double mu, double sigma);
double hamiltonian(double x, double mu, double sigma);
bool step(double &x, Random &rnd, double sigma, double mu, int ntot, int nacc, double delta);
pair <double, double> calculate_hamiltonian(double mu, double sigma, Random &rnd, int ntot, int nacc, double delta, int N, int m, double*AV, double*av_prog, double *av2_prog, double*err_bar);
void calculate_hamilton_file(double mu, double sigma, Random &rnd, int ntot, int nacc, double delta, int N, int m, double*AV, double*av_prog, double *av2_prog, double*err_bar, std:: ofstream& file, std:: ofstream& file_position);
