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
