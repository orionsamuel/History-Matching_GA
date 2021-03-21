#include "genetic.hpp"

genetic_algorithm::genetic_algorithm(int size_population, int n_generations){
    this->size_population = size_population;
    this->n_generations = n_generations;

    for(int i = 0; i < size_population; i++){
        individual ind;
        this->population.push_back(ind);
    }
}

genetic_algorithm::~genetic_algorithm(){

}

void genetic_algorithm::init(){
    firstPopulation();
}

void genetic_algorithm::firstPopulation(){
    DIR* dp = opendir("Output/0");

    if(dp == NULL){
        system("mkdir Output/0");
    }

    for(int i = 0; i < size_population; i++){
        string command = "cp Input/inputDS.dat Output/0/inputDS"+to_string(i)+".dat";
        const char* file = (char*) command.c_str();
        system(file);
    }
}

void genetic_algorithm::othersPopulations(){

}

void genetic_algorithm::fitness(){

}

void genetic_algorithm::crossover(){

}

void genetic_algorithm::mutation(){

}

void genetic_algorithm::sort_rank(){

}