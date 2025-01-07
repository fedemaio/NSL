
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "random.h"
#include "population.h"
#include "tour.h"

using namespace std;

void initialize_random(Random& rnd, int rank) {
    int seed[4];
    int p1, p2;
    ifstream Primes("Primes");
    if (Primes.is_open()) {
        Primes >> p1 >> p2;
    } else {
        cerr << "PROBLEM: Unable to open Primes" << endl;
    }
    Primes.close();

    ifstream input("seed.in");
    string property;
    if (input.is_open()) {
        while (!input.eof()) {
            input >> property;
            if (property == "RANDOMSEED") {
                input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
                rnd.SetRandom(seed, p1, p2);
            }
        }
        input.close();
    } else {
        cerr << "PROBLEM: Unable to open seed.in" << endl;
    }
}

vector<city> read_cities(const string& filename) {
    vector<city> cities;
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return cities;
    }
    
    double lat, lon;
    int index = 0;  // Assegniamo gli indici sequenzialmente
    
    // Leggi tutte le città dal file
    while (in >> lat >> lon) {
        city c;
        c.index = index++;
        c.x = lat;
        c.y = lon;
        cities.push_back(c);
    }
    in.close();
    return cities;
}

vector<int> tour_to_array(tour& t) {
    vector<int> indices;
    indices.reserve(110);
    for (int i = 0; i < 110; i++) {
        indices.push_back(t.operator[](i).index);
    }
    return indices;
}

tour array_to_tour(const vector<int>& arr, const vector<city>& cities) {
    vector<city> ordered_cities;
    for (int i = 0; i < arr.size(); i++) {
        for (const auto& c : cities) {
            if (c.index == arr[i]) {
                ordered_cities.push_back(c);
                break;
            }
        }
    }
    tour new_tour(ordered_cities);
    new_tour.check();  // Verifica la validità del tour
    return new_tour;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // GA Parameters
    const int GENERATIONS = 1000;
    const int POP_SIZE = 50;
    const float CROSSOVER_RATE = 0.7;
    const float MUTATION_RATE_1 = 0.1;
    const float MUTATION_RATE_2 = 0.1;
    const float MUTATION_RATE_3 = 0.1;
    const int MIGRATION_INTERVAL = 100;
    
    // Initialize random number generator
    Random rnd;
    int seed = 1234 + rank;
    initialize_random(rnd, rank);
    
    // Read cities from file
    vector<city> cities = read_cities("cap_prov_ita.dat");
    if (cities.empty()) {
        MPI_Finalize();
        return 1;
    }
    
    // Create initial tour
    tour initial_tour(cities);
    initial_tour.check();  // Verifica la validità del tour iniziale
    
    // Initialize population
    population pop(rnd, POP_SIZE, initial_tour, CROSSOVER_RATE, 
                  MUTATION_RATE_1, MUTATION_RATE_2, MUTATION_RATE_3);
    
    // Main GA loop
    for (int gen = 0; gen < GENERATIONS; gen++) {
        // Evolve population
        pop.step();
        
        // Migration phase
        if (gen % MIGRATION_INTERVAL == 0 && gen > 0) {
            tour best_individual = pop.get_best_tour();
            best_individual.check();  // Verifica la validità del tour
            
            vector<int> send_indices = tour_to_array(best_individual);
            vector<int> recv_indices(110);
            
            int target = (rank + 1) % size;
            int source = (rank - 1 + size) % size;
            
            MPI_Status status;
            MPI_Sendrecv(send_indices.data(), 110, MPI_INT, target, 0,
                        recv_indices.data(), 110, MPI_INT, source, 0,
                        MPI_COMM_WORLD, &status);
            
            tour received_tour = array_to_tour(recv_indices, cities);
            pop.replace_worst_tour(received_tour);
        }
        
        // Print progress
        if (gen % 10 == 0 && rank == 0) {
            tour best = pop.get_best_tour();
            best.check();  // Verifica la validità del tour
            cout << "Generation " << gen << ", Best Length: " << best.path_lenght() << endl;
        }
    }
    
    // Final output
    tour final_best = pop.get_best_tour();
    final_best.check();  // Verifica la validità del tour finale
    double final_length = final_best.path_lenght();
    
    vector<double> all_lengths(size);
    MPI_Gather(&final_length, 1, MPI_DOUBLE, 
               all_lengths.data(), 1, MPI_DOUBLE, 
               0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        cout << "\nFinal Results from all continents:" << endl;
        for (int i = 0; i < size; i++) {
            cout << "Continent " << i << ": " << all_lengths[i] << endl;
        }
        
        double best_length = all_lengths[0];
        int best_continent = 0;
        for (int i = 1; i < size; i++) {
            if (all_lengths[i] < best_length) {
                best_length = all_lengths[i];
                best_continent = i;
            }
        }
        cout << "\nBest overall solution found in continent " 
             << best_continent << " with length " << best_length << endl;
    }
    
    MPI_Finalize();
    return 0;
}