#include <iostream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <random>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include "functions.hpp"

using namespace std;

class genetic_algorithm: public functions{
    private:
    individual population[SIZE_POPULATION];
    result* real_results;
    individual children[((SIZE_POPULATION * CROSSOVER_RATE) / 100)];
    int crossover_rate = ((SIZE_POPULATION * CROSSOVER_RATE) / 100);
    int mutation_rate = ((((SIZE_POPULATION * CROSSOVER_RATE) / 100) * MUTATION_RATE) / 100);
    double min_permeability = 0;
    double max_permeability = 0;

    public:
    genetic_algorithm();
    ~genetic_algorithm();

    void init();
    void readDataset();
    void createPopulations(int idIteration);
    void fitness(int idIteration);
    void crossover();
    void mutation(); 
};
