#include "genetic.hpp"

genetic_algorithm::genetic_algorithm(){
    
}

genetic_algorithm::~genetic_algorithm(){

}

void genetic_algorithm::init(){    
    ifstream result_water("Input/resultadoVazaoAgua.dat", ios::in);
    ifstream result_oil("Input/resultadoVazaoOleo.dat", ios::in);

    string line, line2, content, content2;

    while(!result_water.eof() && !result_oil.eof()){
        getline(result_water, line);
        getline(result_oil, line2);
        content += line;
        content += " ";
        content2 += line2;
        content2 += " ";
    }

    result_water.close();
    result_oil.close();

    vector<string> v{split(content, ' ')};
    vector<string> v2{split(content2, ' ')};
    
    real_results = new result[v.size()];

    for(int i = 0; i < v.size(); i++){
        real_results[i].water = stod(v[i]);
        real_results[i].oil = stod(v2[i]);
    }

    firstPopulation();
    for(int i = 1; i < N_GENERATIONS; i++){
        othersPopulations(i);
    }
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
        string line;
        while(!read_input.eof()){
            getline(read_input, line);
            if(count == 142){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << population[i].porosity << "   " << population[i].permeability_1 << "   " << population[i].permeability_2 << "   " << population[i].permeability_3 << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else{
                write_input << line << endl;
            }
            count++;
        }

        read_input.close();
        write_input.close();

        string command = "mv inputDS_"+to_string(i)+".dat Output/0/";
        const char* file = (char*) command.c_str();
        system(file);
    }

    simulation(0);
    fitness(0);
    sort_rank(population);
}

void genetic_algorithm::othersPopulations(int idIteration){
    crossover();

    string command = "Output/"+to_string(idIteration);
    const char* file = (char*) command.c_str();
    DIR* dp = opendir(file);

    if(dp == NULL){
        command = "mkdir Output/"+to_string(idIteration);
        file = (char*) command.c_str();
        system(file);
    }else{
        command = "rm -f Output/"+to_string(idIteration)+"/*";
        file = (char*) command.c_str();
        system(file);
    }

    for(int i = 0; i < SIZE_POPULATION; i++){
        string command = "cp Input/inputDS.dat Output/"+to_string(idIteration)+"/inputDS_"+to_string(i)+".dat";
        const char* file = (char*) command.c_str();
        system(file);
    }

    for(int i = 0; i < SIZE_POPULATION; i++){
        ifstream read_input("Output/"+to_string(idIteration)+"/inputDS_"+to_string(i)+".dat", ios::in);
        ofstream write_input("inputDS_"+to_string(i)+".dat", ios::out);
        int count = 0;
        string line;
        while(!read_input.eof()){
            getline(read_input, line);
            if(count == 142){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << population[i].porosity << "   " << population[i].permeability_1 << "   " << population[i].permeability_2 << "   " << population[i].permeability_3 << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else{
                write_input << line << endl;
            }
            count++;
        }

        read_input.close();
        write_input.close();

        string command = "mv inputDS_"+to_string(i)+".dat Output/"+to_string(idIteration)+"/";
        const char* file = (char*) command.c_str();
        system(file);
    }

    simulation(idIteration);
    fitness(idIteration);
    sort_rank(population);

}

void genetic_algorithm::fitness(int idIteration){
    for(int i = 0; i < SIZE_POPULATION; i++){
        double rank;

        ifstream result_water("Output/"+to_string(idIteration)+"/vazaoAgua_"+to_string(i)+".dat", ios::in);
        ifstream result_oil("Output/"+to_string(idIteration)+"/vazaoOleo_"+to_string(i)+".dat", ios::in);

        ofstream write_output_water("vazaoAgua_"+to_string(i)+".dat", ios::app);
        ofstream write_output_oil("vazaoOleo_"+to_string(i)+".dat", ios::app);

        string line, line2, content, content2;

        while(!result_water.eof() && !result_oil.eof()){
            getline(result_water, line);
            getline(result_oil, line2);

            write_output_water << line << endl;
            write_output_oil << line2 << endl;

            content += line;
            content += " ";
            content2 += line2;
            content2 += " ";
        }

        result_water.close();
        result_oil.close();

        vector<string> v{split(content, ' ')};
        vector<string> v2{split(content2, ' ')};

        result simulate_results[v.size()];

        for(int j = 0; j < v.size(); j++){
            simulate_results[j].water = stod(v[j]);
            simulate_results[j].oil = stod(v2[j]);
        }

        for(int j = 0; j < N_METRICS; j++){
            if(j == 0){
                for(int k = 0; k < v.size(); k++){
                    rank += pow((real_results[k].water - simulate_results[k].water),2);
                }
                rank *= WATER_WEIGHT;
            }else if(j == 1){
                for(int k = 0; k < v.size(); k++){
                    rank += pow((real_results[k].oil - simulate_results[k].oil),2);
                }
                rank *= OIL_WEIGHT;
            }
            rank += rank;            
        }

        rank = sqrt((rank / (v.size() * 2)));

        population[i].error_rank = rank;

        write_output_water << endl;
        write_output_water << endl;
        write_output_water << endl;
        write_output_water << "Taxa de Erro: " << rank << endl;

        write_output_oil << endl;
        write_output_oil << endl;
        write_output_oil << endl;
        write_output_oil << rank << endl;

        write_output_water.close();
        write_output_oil.close();

        string command = "mv vazaoAgua_"+to_string(i)+".dat Output/"+to_string(idIteration)+"/";
        const char* file = (char*) command.c_str();
        system(file);

        command = "mv vazaoOleo_"+to_string(i)+".dat Output/"+to_string(idIteration)+"/";
        file = (char*) command.c_str();
        system(file);

    }
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
