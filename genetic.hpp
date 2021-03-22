#include <iostream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <random>
#include <iomanip>
#include <vector>
#include "functions.hpp"

#define MIN_POROSITY 0.0000000E+00
#define MAX_POROSITY 9.7229995E-02

#define MIN_PERMEABILITY 0.0000000E+00
#define MAX_PERMEABILITY 4.4235002E-02

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
