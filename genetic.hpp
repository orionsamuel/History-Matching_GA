#include <iostream>
#include <vector>
#include "utils.hpp"

using namespace std;

class genetic_algorithm{
    private:
    int size_population;
    int n_individuals;
    vector <individual> population;

    public:
    genetic_algorithm(int size_population, int n_inviduals);
    ~genetic_algorithm();

    void init();
    void firstPopulation();
    void othersPopulations();
    void fitness();
    void crossover();
    void mutation();

};