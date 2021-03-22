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
        system("rm Output/0/*");
    }

    for(int i = 0; i < SIZE_POPULATION; i++){
        string command = "cp Input/inputDS.dat Output/0/inputDS_"+to_string(i)+".dat";
        const char* file = (char*) command.c_str();
        system(file);
    }

    for(int i = 0; i < SIZE_POPULATION; i++){
        population[i].porosity = rand(MIN_POROSITY, MAX_POROSITY);
        population[i].permeability_1 = rand(MIN_PERMEABILITY, MAX_PERMEABILITY);
        population[i].permeability_2 = rand(MIN_PERMEABILITY, MAX_PERMEABILITY);
        population[i].permeability_3 = rand(MIN_PERMEABILITY, MAX_PERMEABILITY);
    }
}

void genetic_algorithm::othersPopulations(){

}

void genetic_algorithm::simulation(int idIteration){
    system("rm -f output_simulation");
    for(int i = 0; i < SIZE_POPULATION; i++){
        string command = "cp Output/"+to_string(idIteration)+"/inputDS_"+to_string(i)+".dat ../Codigo_Bifasico_Slab/simulacoes/dev/inputDS.dat";
        const char* file = (char*) command.c_str();
        system(file);
        system("./../Codigo_Bifasico_Slab/rodarSimulador.sh >> output_simulation 2>> output_simulation");
        command = "cp ../Codigo_Bifasico_Slab/simulacoes/dev/out/resultadoVazaoAgua.dat Output/"+to_string(idIteration)+"/vazaoAgua_"+to_string(i)+".dat";
        file = (char*) command.c_str();
        system(file);
        command = "cp ../Codigo_Bifasico_Slab/simulacoes/dev/out/resultadoVazaoOleo.dat Output/"+to_string(idIteration)+"/vazaoOleo_"+to_string(i)+".dat";
        file = (char*) command.c_str();
        system(file);
    }
}

void genetic_algorithm::fitness(){

}

void genetic_algorithm::crossover(){

}

void genetic_algorithm::mutation(){

}

void genetic_algorithm::sort_rank(){

}

float genetic_algorithm::rand(float min, float max){
    random_device rd;
    default_random_engine eng(rd());
    uniform_real_distribution<float>distr(min, max);

    float num = distr(eng);

    return num;
}