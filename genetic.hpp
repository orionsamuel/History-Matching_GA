#include <iostream>
#include <vector>
#include <string>
 #include <sys/types.h>
 #include <dirent.h>
#include "utils.hpp"

using namespace std;

class genetic_algorithm{
    private:
    int size_population;
    int n_generations;
    vector <individual> population;

    public:
    genetic_algorithm(int size_population, int n_generations);
    ~genetic_algorithm();

    void init();
    void firstPopulation();
    void othersPopulations();
    void fitness();
    void crossover();
    void mutation();
    void sort_rank();

};