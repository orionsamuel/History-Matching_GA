#include <iostream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include "utils.hpp"

using namespace std;

class genetic_algorithm{
    private:
    individual population[SIZE_POPULATION];

    public:
    genetic_algorithm();
    ~genetic_algorithm();

    void init();
    void firstPopulation();
    void othersPopulations();
    void simulation(int idIteration);
    void fitness();
    void crossover();
    void mutation();
    void sort_rank();

};