#include <iostream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <random>
#include <iomanip>
#include <vector>
#include "functions.hpp"

using namespace std;

class genetic_algorithm: public functions{
    private:
    individual population[SIZE_POPULATION];
    vector <result> results;
    int crossover_rate = ((SIZE_POPULATION * CROSSOVER_RATE) / 100);
    int mutation_rate = ((SIZE_POPULATION * MUTATION_RATE) / 100);

    public:
    genetic_algorithm();
    ~genetic_algorithm();

    void init();
    void firstPopulation();
    void othersPopulations();
    void fitness();
    void crossover();
    void mutation(); 
};
