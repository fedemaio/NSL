#include <iostream>
#include <vector>
#include <mpi.h>
#include <fstream>
#include <string>
#include "population.h"
#include "tour.h"
#include "random.h"

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

void run_parallel_ga(Random& rnd, const tour& base_tour, int num_chromosomes, int num_generations, int migration_interval, int rank, int size) {
    float crossover_prob = 0.8;
    float mutation_prob_1 = 0.1;
    float mutation_prob_2 = 0.1;
    float mutation_prob_3 = 0.1;

    // Initialize population for each MPI process
    population pop(rnd, num_chromosomes, base_tour, crossover_prob, mutation_prob_1, mutation_prob_2, mutation_prob_3);

    for (int gen = 0; gen < num_generations; ++gen) {
        pop.step(); // Perform one generation step

        // Perform migration every migration_interval generations
        if (gen % migration_interval == 0) {
            vector<int> best_tour_serialized = pop.get_best_tour().serialize();
            int num_cities = best_tour_serialized.size();
            vector<int> received_tour_serialized(num_cities);

            // Exchange best individuals randomly between processes
            int partner_rank = rnd.Rannyu(0, size);
            MPI_Sendrecv(best_tour_serialized.data(), num_cities, MPI_INT, partner_rank, 0,
                         received_tour_serialized.data(), num_cities, MPI_INT, partner_rank, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            tour received_tour(base_tour);
            received_tour.deserialize(received_tour_serialized);
            pop.replace_worst_tour(received_tour);
        }
    }

    // Gather results to rank 0
    tour best_tour = pop.get_best_tour();
    double best_length = best_tour.path_lenght();
    double global_best_length;

    MPI_Reduce(&best_length, &global_best_length, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Global best path length: " << global_best_length << endl;

        // Write best tour to file
        ofstream output_file("best_tour.out");
        if (output_file.is_open()) {
            const vector<city>& cities = base_tour.get_cities();
            for (const int& idx : best_tour.serialize()) {
                const city& c = cities[idx];
                output_file << c.index << " " << c.x << " " << c.y << endl;
            }
            output_file.close();
        } else {
            cerr << "PROBLEM: Unable to open best_tour.dat for writing" << endl;
        }
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Seed random number generator uniquely per process
    Random rnd;
    initialize_random(rnd, rank);

    // Load data from file (assuming cap_prov_ita.dat format is correct)
    ifstream input_file("cap_prov_ita.dat");
    vector<city> cities;
    int idx;
    double x, y;

    while (input_file >> idx >> x >> y) {
        cities.emplace_back(idx, x, y);
    }

    tour base_tour(cities);

    int num_chromosomes = 100; // Number of chromosomes per population
    int num_generations = 1000; // Total number of generations
    int migration_interval = 50; // Migration interval in generations

    run_parallel_ga(rnd, base_tour, num_chromosomes, num_generations, migration_interval, rank, size);

    MPI_Finalize();
    return 0;
}
