#include <iostream>
#include <vector>
#include "utils.hpp"

using namespace std;

class genetic_algorithm{
    public:
    int size_population;
    int n_individuals;
    vector <individual> population;

    genetic_algorithm(int size_population, int n_inviduals);
    ~genetic_algorithm();

    private:
    void init();
    void firstPopulation();
    void othersPopulations();
    void fitness();
    void crossover();
    void mutation();

};