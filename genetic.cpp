#include "genetic.hpp"

genetic_algorithm::genetic_algorithm(){
    
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
    }else{
        system("rm -f Output/0/*");
    }

    for(int i = 0; i < SIZE_POPULATION; i++){
        string command = "cp Input/inputDS.dat Output/0/inputDS_"+to_string(i)+".dat";
        const char* file = (char*) command.c_str();
        system(file);
    }

    for(int i = 0; i < SIZE_POPULATION; i++){
        population[i].porosity = rand_double(MIN_POROSITY, MAX_POROSITY);
        population[i].permeability_1 = rand_double(MIN_POROSITY, MAX_PERMEABILITY);
        population[i].permeability_2 = rand_double(MIN_PERMEABILITY, MAX_PERMEABILITY);
        population[i].permeability_3 = rand_double(MIN_PERMEABILITY, MAX_PERMEABILITY);
    }

    for(int i = 0; i < SIZE_POPULATION; i++){
        ifstream read_input("Output/0/inputDS_"+to_string(i)+".dat", ios::in);
        ofstream write_input("inputDS_"+to_string(i)+".dat", ios::out);
        int count = 0;
        string linha;
        while(!read_input.eof()){
            getline(read_input, linha);
            if(count == 142){
                vector<string> v{split(linha, ' ')};
                write_input << "         " << v[0] << "   " << scientific << population[i].porosity << "   " << population[i].permeability_1 << "   " << population[i].permeability_2 << "   " << population[i].permeability_3 << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else{
                write_input << linha << endl;
            }
            count++;
        }
        string command = "mv inputDS_"+to_string(i)+".dat Output/0/";
        const char* file = (char*) command.c_str();
        system(file);
    }
    simulation(0);
}

void genetic_algorithm::othersPopulations(){

}

void genetic_algorithm::fitness(){

}

void genetic_algorithm::crossover(){
    vector <individual> childrens;

    int count = 0;
    while(count < (crossover_rate / 2)){
        individual children_1, children_2;

        children_1.porosity = population[count].porosity;
        children_1.permeability_1 = population[count + 1].permeability_1;
        children_1.permeability_2 = population[count + 1].permeability_2;
        children_1.permeability_3 = population[count + 1].permeability_3;

        childrens.push_back((children_1));

        children_2.porosity = population[count + 1].porosity;
        children_2.permeability_1 = population[count].permeability_1;
        children_2.permeability_2 = population[count].permeability_2;
        children_2.permeability_3 = population[count].permeability_3;

        childrens.push_back((children_2));

        count = count + 2;
    }

    mutation(childrens);

    count = 0;
    for(int i = crossover_rate; i > 0; i--){
        population[SIZE_POPULATION - i].porosity = childrens[count].porosity;
        population[SIZE_POPULATION - i].permeability_1 = childrens[count].permeability_1;
        population[SIZE_POPULATION - i].permeability_2 = childrens[count].permeability_2;
        population[SIZE_POPULATION - i].permeability_3 = childrens[count].permeability_3;

        count++;
    }
    
}

void genetic_algorithm::mutation(vector <individual>& childrens){
    for(int i = 0; i < childrens.size(); i++){
        srand (time(NULL));
        int percent = rand() % 2;
        int tunning = rand() % 1;

        if(percent == 0){
            double valuePorosity = ((childrens[i].porosity * 5) / 100);
            double valuePermeability_1 = ((childrens[i].permeability_1 * 5) / 100);
            double valuePermeability_2 = ((childrens[i].permeability_2 * 5) / 100);
            double valuePermeability_3 = ((childrens[i].permeability_2 * 5) / 100);
            if(tunning = 0){
                childrens[i].porosity = max(MAX_POROSITY, (childrens[i].porosity + valuePorosity));
                childrens[i].permeability_1 = max(MAX_POROSITY, (childrens[i].permeability_1 + valuePermeability_1));
                childrens[i].permeability_2 = max(MAX_POROSITY, (childrens[i].permeability_2 + valuePermeability_2));
                childrens[i].permeability_3 = max(MAX_POROSITY, (childrens[i].permeability_3 + valuePermeability_3));
            }else{
                childrens[i].porosity = min(MAX_POROSITY, (childrens[i].porosity + valuePorosity));
                childrens[i].permeability_1 = min(MAX_POROSITY, (childrens[i].permeability_1 + valuePermeability_1));
                childrens[i].permeability_2 = min(MAX_POROSITY, (childrens[i].permeability_2 + valuePermeability_2));
                childrens[i].permeability_3 = min(MAX_POROSITY, (childrens[i].permeability_3 + valuePermeability_3));
            }
        }else if(percent == 1){
            double valuePorosity = ((childrens[i].porosity * 10) / 100);
            double valuePermeability_1 = ((childrens[i].permeability_1 * 10) / 100);
            double valuePermeability_2 = ((childrens[i].permeability_2 * 10) / 100);
            double valuePermeability_3 = ((childrens[i].permeability_2 * 10) / 100);
            if(tunning = 0){
                childrens[i].porosity = max(MAX_POROSITY, (childrens[i].porosity + valuePorosity));
                childrens[i].permeability_1 = max(MAX_POROSITY, (childrens[i].permeability_1 + valuePermeability_1));
                childrens[i].permeability_2 = max(MAX_POROSITY, (childrens[i].permeability_2 + valuePermeability_2));
                childrens[i].permeability_3 = max(MAX_POROSITY, (childrens[i].permeability_3 + valuePermeability_3));
            }else{
                childrens[i].porosity = min(MAX_POROSITY, (childrens[i].porosity + valuePorosity));
                childrens[i].permeability_1 = min(MAX_POROSITY, (childrens[i].permeability_1 + valuePermeability_1));
                childrens[i].permeability_2 = min(MAX_POROSITY, (childrens[i].permeability_2 + valuePermeability_2));
                childrens[i].permeability_3 = min(MAX_POROSITY, (childrens[i].permeability_3 + valuePermeability_3));
            }
        }else if(percent == 2){
            double valuePorosity = ((childrens[i].porosity * 20) / 100);
            double valuePermeability_1 = ((childrens[i].permeability_1 * 20) / 100);
            double valuePermeability_2 = ((childrens[i].permeability_2 * 20) / 100);
            double valuePermeability_3 = ((childrens[i].permeability_2 * 20) / 100);
            if(tunning = 0){
                childrens[i].porosity = max(MAX_POROSITY, (childrens[i].porosity + valuePorosity));
                childrens[i].permeability_1 = max(MAX_POROSITY, (childrens[i].permeability_1 + valuePermeability_1));
                childrens[i].permeability_2 = max(MAX_POROSITY, (childrens[i].permeability_2 + valuePermeability_2));
                childrens[i].permeability_3 = max(MAX_POROSITY, (childrens[i].permeability_3 + valuePermeability_3));
            }else{
                childrens[i].porosity = min(MAX_POROSITY, (childrens[i].porosity + valuePorosity));
                childrens[i].permeability_1 = min(MAX_POROSITY, (childrens[i].permeability_1 + valuePermeability_1));
                childrens[i].permeability_2 = min(MAX_POROSITY, (childrens[i].permeability_2 + valuePermeability_2));
                childrens[i].permeability_3 = min(MAX_POROSITY, (childrens[i].permeability_3 + valuePermeability_3));
            }
        }
    }
}
