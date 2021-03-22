#include "genetic.hpp"

genetic_algorithm::genetic_algorithm(){
    
}

genetic_algorithm::~genetic_algorithm(){

}

void genetic_algorithm::init(){
    firstPopulation();
    simulation(0);
}

void genetic_algorithm::firstPopulation(){
    DIR* dp = opendir("Output/0");

    if(dp == NULL){
        system("mkdir Output/0");
    }else{
        system("rm -f Output/0/*");
    }

    for(int i = 0; i < SIZE_POPULATION; i++){
        string command = "cp Input/inputDS.dat Output/0/inputDS_"+to_string(i)+".dat";
        const char* file = (char*) command.c_str();
        system(file);
    }

    for(int i = 0; i < SIZE_POPULATION; i++){
        population[i].porosity = rand(MIN_POROSITY, MAX_POROSITY);
        population[i].permeability_1 = rand(MIN_POROSITY, MAX_PERMEABILITY);
        population[i].permeability_2 = rand(MIN_PERMEABILITY, MAX_PERMEABILITY);
        population[i].permeability_3 = rand(MIN_PERMEABILITY, MAX_PERMEABILITY);
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
