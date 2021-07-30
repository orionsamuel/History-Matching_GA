#include "../include/genetic.hpp"

genetic_algorithm::genetic_algorithm(){
    srand (time(0));
}

genetic_algorithm::~genetic_algorithm(){

}

void genetic_algorithm::init(){    
    DIR* dp = opendir("../Output/");

    if(dp == NULL){
        system("mkdir ../Output/");
    }
    system("rm -r -f ../Output/*");

    ifstream result_water("../Input/resultadoVazaoAgua.dat", ios::in);
    ifstream result_oil("../Input/resultadoVazaoOleo.dat", ios::in);

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
        this->real_results[i].water = stod(v[i]);
        this->real_results[i].oil = stod(v2[i]);
    }

    firstPopulation();
    int count = 1;
    while(count < N_GENERATIONS){
        otherPopulations(count);
        count++;
    }
}

void genetic_algorithm::firstPopulation(){
    DIR* dp = opendir("../Output/0");

    if(dp == NULL){
        system("mkdir ../Output/0");
    }else{
        system("rm -f ../Output/0/*");
    }

    vector<individual> dataset;

    //Leitura do dataset e criação do arquivo da população 0
    ifstream read_input("../Dataset/history_matching.csv", ios::in);
    ofstream write_output("../Output/0/0.csv", ios::out);

    string line;

    //Passando os valores contidos no arquivo para um vector do tipo individuo
    int count = 0;
    while(!read_input.eof()){
        getline(read_input, line);
        if(count == 0){
            write_output << line << endl;
        }else if(count < 1801){
            vector<string> v{split(line, ',')};
            dataset[0].porosity[0];
            dataset.push_back({{stod(v[0]),stod(v[4]),stod(v[8]),stod(v[12]),stod(v[16])}, 
            {stod(v[1]),stod(v[5]),stod(v[9]),stod(v[13]),stod(v[17])}, 
            {stod(v[2]),stod(v[6]),stod(v[10]),stod(v[14]),stod(v[18])}, 
            {stod(v[3]),stod(v[7]),stod(v[11]),stod(v[15]),stod(v[19])}, stod(v[20])});
        }
        count++;        
    }

    read_input.close();

    sort(begin(dataset), end(dataset), compare);

    for(int i = 0; i < SIZE_POPULATION; i++){
        write_output << scientific << dataset[i].porosity[0] << "," 
        << dataset[i].permeability_1[0] << "," << dataset[i].permeability_2[0] 
        << "," << dataset[i].permeability_3[0] << "," << dataset[i].porosity[1] 
        << "," << dataset[i].permeability_1[1] << "," << dataset[i].permeability_2[1] 
        << "," << dataset[i].permeability_3[1] << "," << dataset[i].porosity[2] << "," 
        << dataset[i].permeability_1[2] << "," << dataset[i].permeability_2[2] 
        << "," << dataset[i].permeability_3[2] << "," << dataset[i].porosity[3] << "," 
        << dataset[i].permeability_1[3] << "," << dataset[i].permeability_2[3] 
        << "," << dataset[i].permeability_3[3] << "," << dataset[i].porosity[4] << "," 
        << dataset[i].permeability_1[4] << "," << dataset[i].permeability_2[4] 
        << "," << dataset[i].permeability_3[4] << "," << dataset[i].error_rank << endl;
    }

    write_output.close();

    ifstream read_csv("../Output/0/0.csv", ios::in);

    string decimal[2];
    line = "";
    
    //Pega a casa decimal da permeabilidade da menor taxa de erro, para gerar os valores mínimos e máximos de permeabilidade
    count = 0;
    while(!read_csv.eof()){
        getline(read_csv, line);
        if(count == 1){
            vector<string> v{split(line, ',')};
            decimal[0] = v[1][10];
            decimal[0] += v[1][11];
            decimal[1] += v[9][10];
            decimal[1] += v[9][11];
            cout << endl;
        }

        count++;        
    }

    read_csv.close();
    
    string minPermeability[2];
    string maxPermeability[2];
    minPermeability[0] = "1.000000e-"+decimal[0];
    maxPermeability[0] = "3.000000e-"+decimal[0];
    minPermeability[1] = "1.000000e-"+decimal[1];
    maxPermeability[1] = "3.000000e-"+decimal[1];

    this->min_permeability[0] = stod(minPermeability[0]);
    this->max_permeability[0] = stod(maxPermeability[0]);
    this->min_permeability[1] = stod(minPermeability[1]);
    this->max_permeability[1] = stod(maxPermeability[1]);

    system("rm ../Output/0/0.csv");

    for(int i = 0; i < SIZE_POPULATION; i++){
        string command = "cp ../Input/inputDS.dat ../Output/0/inputDS_"+to_string(i)+".dat";
        const char* file = (char*) command.c_str();
        system(file);
    }

    for(int i = 0; i < SIZE_POPULATION; i++){
        for(int j = 0; j < 5; j++){
            this->population[i].porosity[j] = dataset[i].porosity[j];
            this->population[i].permeability_1[j] = dataset[i].permeability_1[j];
            this->population[i].permeability_2[j] = dataset[i].permeability_2[j];
            this->population[i].permeability_3[j] = dataset[i].permeability_3[j];
            this->population[i].error_rank = dataset[i].error_rank;
        }
    }

    /*for(int i = 0; i < SIZE_POPULATION; i++){
        this->population[i].porosity[0] = rand_double(MIN_POROSITY, MAX_POROSITY);
        this->population[i].porosity[1] = dataset[i].porosity[1];
        this->population[i].porosity[2] = rand_double(MIN_POROSITY, MAX_POROSITY);
        this->population[i].porosity[3] = rand_double(MIN_POROSITY, MAX_POROSITY);
        this->population[i].porosity[4] = rand_double(MIN_POROSITY, MAX_POROSITY);
        this->population[i].permeability_1[0] = rand_double(this->min_permeability[0], this->max_permeability[0]);
        this->population[i].permeability_1[1] = dataset[i].permeability_1[1];
        this->population[i].permeability_1[2] = rand_double(this->min_permeability[1], this->max_permeability[1]);
        this->population[i].permeability_1[3] = rand_double(this->min_permeability[1], this->max_permeability[1]);
        this->population[i].permeability_1[4] = rand_double(this->min_permeability[0], this->max_permeability[0]);
        this->population[i].permeability_2[0] = rand_double(this->min_permeability[0], this->max_permeability[0]);
        this->population[i].permeability_2[1] = dataset[i].permeability_2[1];
        this->population[i].permeability_2[2] = rand_double(this->min_permeability[1], this->max_permeability[1]);
        this->population[i].permeability_2[3] = rand_double(this->min_permeability[1], this->max_permeability[1]);
        this->population[i].permeability_2[4] = rand_double(this->min_permeability[0], this->max_permeability[0]);
        this->population[i].permeability_3[0] = rand_double(this->min_permeability[0], this->max_permeability[0]);
        this->population[i].permeability_3[1] = dataset[i].permeability_3[1];
        this->population[i].permeability_3[2] = rand_double(this->min_permeability[1], this->max_permeability[1]);
        this->population[i].permeability_3[3] = rand_double(this->min_permeability[1], this->max_permeability[1]);
        this->population[i].permeability_3[4] = rand_double(this->min_permeability[0], this->max_permeability[0]);
    }*/

    for(int i = 0; i < SIZE_POPULATION; i++){
        ifstream read_input("../Output/0/inputDS_"+to_string(i)+".dat", ios::in);
        ofstream write_input("../inputDS_"+to_string(i)+".dat", ios::out);
        int count = 0;
        line = "";
        while(!read_input.eof()){
            getline(read_input, line);
            if(count == 142){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[0] << "   " << this->population[i].permeability_1[0] << "   " << this->population[i].permeability_2[0] << "   " << this->population[i].permeability_3[0] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 143){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[1] << "   " << this->population[i].permeability_1[1] << "   " << this->population[i].permeability_2[1] << "   " << this->population[i].permeability_3[1] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 144){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[2] << "   " << this->population[i].permeability_1[2] << "   " << this->population[i].permeability_2[2] << "   " << this->population[i].permeability_3[2] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 145){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[3] << "   " << this->population[i].permeability_1[3] << "   " << this->population[i].permeability_2[3] << "   " << this->population[i].permeability_3[3] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 146){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[4] << "   " << this->population[i].permeability_1[4] << "   " << this->population[i].permeability_2[4] << "   " << this->population[i].permeability_3[4] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else{
                write_input << line << endl;
            }
            count++;
        }

        read_input.close();
        write_input.close();

        string command = "mv ../inputDS_"+to_string(i)+".dat ../Output/0/";
        const char* file = (char*) command.c_str();
        system(file);
    }

    /*simulation(0);
    fitness(0);
    sort(begin(this->population), end(this->population), compare);*/

    ofstream write_error("../Output/0/error.txt", ios::out);
    
    for(int i = 0; i < SIZE_POPULATION; i++){
        write_error << scientific << this->population[i].error_rank << endl;
    }

    write_error.close();

    /*for(int i = 0; i < SIZE_POPULATION; i++){
        ifstream read_input("../Output/0/inputDS_"+to_string(i)+".dat", ios::in);
        ofstream write_input("../inputDS_"+to_string(i)+".dat", ios::out);
        int count = 0;
        line = "";
        while(!read_input.eof()){
            getline(read_input, line);
            if(count == 142){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[0] << "   " << this->population[i].permeability_1[0] << "   " << this->population[i].permeability_2[0] << "   " << this->population[i].permeability_3[0] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 143){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[1] << "   " << this->population[i].permeability_1[1] << "   " << this->population[i].permeability_2[1] << "   " << this->population[i].permeability_3[1] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 144){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[2] << "   " << this->population[i].permeability_1[2] << "   " << this->population[i].permeability_2[2] << "   " << this->population[i].permeability_3[2] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 145){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[3] << "   " << this->population[i].permeability_1[3] << "   " << this->population[i].permeability_2[3] << "   " << this->population[i].permeability_3[3] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 146){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[4] << "   " << this->population[i].permeability_1[4] << "   " << this->population[i].permeability_2[4] << "   " << this->population[i].permeability_3[4] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else{
                write_input << line << endl;
            }
            count++;
        }

        read_input.close();
        write_input.close();

        string command = "mv ../inputDS_"+to_string(i)+".dat ../Output/0/";
        const char* file = (char*) command.c_str();
        system(file);
    }*/
}

void genetic_algorithm::otherPopulations(int idIteration){
    crossover();

    string command = "../Output/"+to_string(idIteration);
    const char* file = (char*) command.c_str();
    DIR* dp = opendir(file);

    if(dp == NULL){
        command = "mkdir ../Output/"+to_string(idIteration);
        string command2 = "mkdir ../Output/"+to_string(idIteration)+"/agua";
        string command3 = "mkdir ../Output/"+to_string(idIteration)+"/oleo";
        file = (char*) command.c_str();
        const char* file2 = (char*) command2.c_str();
        const char* file3 = (char*) command3.c_str();
        system(file);
        system(file2);
        system(file3);
    }else{
        command = "rm -f ../Output/"+to_string(idIteration)+"/*";
        file = (char*) command.c_str();
        system(file);
    }
        

    for(int i = 0; i < SIZE_POPULATION; i++){
        string command = "cp ../Input/inputDS.dat ../Output/"+to_string(idIteration)+"/inputDS_"+to_string(i)+".dat";
        const char* file = (char*) command.c_str();
        system(file);
    }

    for(int i = 0; i < SIZE_POPULATION; i++){
        ifstream read_input("../Output/"+to_string(idIteration)+"/inputDS_"+to_string(i)+".dat", ios::in);
        ofstream write_input("../inputDS_"+to_string(i)+".dat", ios::out);
        int count = 0;
        string line;
        while(!read_input.eof()){
            getline(read_input, line);
            if(count == 142){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[0] << "   " << this->population[i].permeability_1[0] << "   " << this->population[i].permeability_2[0] << "   " << this->population[i].permeability_3[0] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 143){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[1] << "   " << this->population[i].permeability_1[1] << "   " << this->population[i].permeability_2[1] << "   " << this->population[i].permeability_3[1] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 144){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[2] << "   " << this->population[i].permeability_1[2] << "   " << this->population[i].permeability_2[2] << "   " << this->population[i].permeability_3[2] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 145){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[3] << "   " << this->population[i].permeability_1[3] << "   " << this->population[i].permeability_2[3] << "   " << this->population[i].permeability_3[3] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 146){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[4] << "   " << this->population[i].permeability_1[4] << "   " << this->population[i].permeability_2[4] << "   " << this->population[i].permeability_3[4] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else{
                write_input << line << endl;
            }
            count++;
        }

        read_input.close();
        write_input.close();

        string command = "mv ../inputDS_"+to_string(i)+".dat ../Output/"+to_string(idIteration)+"/";
        const char* file = (char*) command.c_str();
        system(file);
    }

    simulation(idIteration);
    fitness(idIteration);
    sort(begin(this->population), end(this->population), compare);

    ofstream write_error("../Output/"+to_string(idIteration)+"/error.txt", ios::out);
    
    for(int i = 0; i < SIZE_POPULATION; i++){
        write_error << scientific << this->population[i].error_rank << endl;
    }

    write_error.close();

    for(int i = 0; i < SIZE_POPULATION; i++){
        ifstream read_input("../Output/"+to_string(idIteration)+"/inputDS_"+to_string(i)+".dat", ios::in);
        ofstream write_input("../inputDS_"+to_string(i)+".dat", ios::out);
        int count = 0;
        string line;
        while(!read_input.eof()){
            getline(read_input, line);
            if(count == 142){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[0] << "   " << this->population[i].permeability_1[0] << "   " << this->population[i].permeability_2[0] << "   " << this->population[i].permeability_3[0] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 143){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[1] << "   " << this->population[i].permeability_1[1] << "   " << this->population[i].permeability_2[1] << "   " << this->population[i].permeability_3[1] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 144){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[2] << "   " << this->population[i].permeability_1[2] << "   " << this->population[i].permeability_2[2] << "   " << this->population[i].permeability_3[2] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 145){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[3] << "   " << this->population[i].permeability_1[3] << "   " << this->population[i].permeability_2[3] << "   " << this->population[i].permeability_3[3] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else if(count == 146){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity[4] << "   " << this->population[i].permeability_1[4] << "   " << this->population[i].permeability_2[4] << "   " << this->population[i].permeability_3[4] << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
            }else{
                write_input << line << endl;
            }
            count++;
        }

        read_input.close();
        write_input.close();

        string command = "mv ../inputDS_"+to_string(i)+".dat ../Output/"+to_string(idIteration)+"/";
        const char* file = (char*) command.c_str();
        system(file);
    }
}

void genetic_algorithm::fitness(int idIteration){

    for(int i = 0; i < SIZE_POPULATION; i++){
        double rank;

        ifstream result_water("../Output/"+to_string(idIteration)+"/agua/vazaoAgua_"+to_string(i)+".dat", ios::in);
        ifstream result_oil("../Output/"+to_string(idIteration)+"/oleo/vazaoOleo_"+to_string(i)+".dat", ios::in);

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

        result simulate_results[v.size()];

        for(int j = 0; j < v.size(); j++){
            simulate_results[j].water = stod(v[j]);
            simulate_results[j].oil = stod(v2[j]);
        }

        for(int j = 0; j < N_METRICS; j++){
            if(j == 0){
                for(int k = 0; k < v.size(); k++){
                    rank += pow((this->real_results[k].water - simulate_results[k].water),2);
                }
                rank *= WATER_WEIGHT;
            }else if(j == 1){
                for(int k = 0; k < v.size(); k++){
                    rank += pow((this->real_results[k].oil - simulate_results[k].oil),2);
                }
                rank *= OIL_WEIGHT;
            }           
        }

        rank = sqrt((rank / (v.size() * 2)));

        this->population[i].error_rank = rank;
    }
}

void genetic_algorithm::crossover(){
    for(int i = 0; i < crossover_rate; i++){
        for(int j = 0; j < 5; j++){
            this->children[i].porosity[j] = 0;
            this->children[i].permeability_1[j] = 0;
            this->children[i].permeability_2[j] = 0;
            this->children[i].permeability_3[j] = 0;
        }
    }

    //Escolhe o ponto de corte
    int percent = rand() % 3;
    if(percent == 0){
        int count = 0;
        while(count < crossover_rate){
            for(int i = 0; i < 5; i++){
                this->children[count].porosity[i] = this->population[count].porosity[i];
                this->children[count].permeability_1[i] = this->population[count + 1].permeability_1[i];
                this->children[count].permeability_2[i] = this->population[count + 1].permeability_2[i];
                this->children[count].permeability_3[i] = this->population[count + 1].permeability_3[i];

                this->children[count + 1].porosity[i] = this->population[count + 1].porosity[i];
                this->children[count + 1].permeability_1[i] = this->population[count].permeability_1[i];
                this->children[count + 1].permeability_2[i] = this->population[count].permeability_2[i];
                this->children[count + 1].permeability_3[i] = this->population[count].permeability_3[i];
            }
            count = count + 2;
        }
    }else if(percent == 1){
        int count = 0;
        while(count < crossover_rate){
            for(int i = 0; i < 5; i++){
                this->children[count].porosity[i] = this->population[count].porosity[i];
                this->children[count].permeability_1[i] = this->population[count].permeability_1[i];
                this->children[count].permeability_2[i] = this->population[count + 1].permeability_2[i];
                this->children[count].permeability_3[i] = this->population[count + 1].permeability_3[i];

                this->children[count + 1].porosity[i] = this->population[count + 1].porosity[i];
                this->children[count + 1].permeability_1[i] = this->population[count + 1].permeability_1[i];
                this->children[count + 1].permeability_2[i] = this->population[count].permeability_2[i];
                this->children[count + 1].permeability_3[i] = this->population[count].permeability_3[i];
            }
            count = count + 2;
        }
    }else if(percent == 2){
        int count = 0;
        while(count < crossover_rate){
            for(int i = 0; i < 5; i++){
                this->children[count].porosity[i] = this->population[count].porosity[i];
                this->children[count].permeability_1[i] = this->population[count].permeability_1[i];
                this->children[count].permeability_2[i] = this->population[count].permeability_2[i];
                this->children[count].permeability_3[i] = this->population[count + 1].permeability_3[i];

                this->children[count + 1].porosity[i] = this->population[count + 1].porosity[i];
                this->children[count + 1].permeability_1[i] = this->population[count + 1].permeability_1[i];
                this->children[count + 1].permeability_2[i] = this->population[count + 1].permeability_2[i];
                this->children[count + 1].permeability_3[i] = this->population[count].permeability_3[i];
            }
            count = count + 2;
        }
    }

    mutation();

    int count = 0;
    for(int i = (SIZE_POPULATION - crossover_rate); i < SIZE_POPULATION; i++){
        for(int j = 0; j < 5; j++){
            this->population[i].porosity[j] = this->children[count].porosity[j];
            this->population[i].permeability_1[j] = this->children[count].permeability_1[j];
            this->population[i].permeability_2[j] = this->children[count].permeability_2[j];
            this->population[i].permeability_3[j] = this->children[count].permeability_3[j];
        }
        count++;
    }
    
}

void genetic_algorithm::mutation(){
    double valuePorosity;
    double valuePermeability_1;
    double valuePermeability_2;
    double valuePermeability_3;

     for(int i = 0; i < mutation_rate; i++){
        int percent = rand() % 3;
        int tunning = rand() % 2;

        if(percent == 0){
            for(int j = 0; j < 5; j++){
                valuePorosity = ((this->children[i].porosity[j] * 5) / 100);
                valuePermeability_1 = ((this->children[i].permeability_1[j] * 5) / 100);
                valuePermeability_2 = ((this->children[i].permeability_2[j] * 5) / 100);
                valuePermeability_3 = ((this->children[i].permeability_2[j] * 5) / 100);
                if(j == 1){
                    break;
                }else if((j == 0) || (j == 4)){
                    if(tunning = 0){
                        this->children[i].porosity[j] = min(MAX_POROSITY, (this->children[i].porosity[j] + valuePorosity));
                        this->children[i].permeability_1[j] = min(this->max_permeability[0], (this->children[i].permeability_1[j] + valuePermeability_1));
                        this->children[i].permeability_2[j] = min(this->max_permeability[0], (this->children[i].permeability_2[j] + valuePermeability_2));
                        this->children[i].permeability_3[j] = min(this->max_permeability[0], (this->children[i].permeability_3[j] + valuePermeability_3));
                    }else{
                        this->children[i].porosity[j] = max(MIN_POROSITY, (this->children[i].porosity[j] - valuePorosity));
                        this->children[i].permeability_1[j] = max(this->min_permeability[0], (this->children[i].permeability_1[j] - valuePermeability_1));
                        this->children[i].permeability_2[j] = max(this->min_permeability[0], (this->children[i].permeability_2[j] - valuePermeability_2));
                        this->children[i].permeability_3[j] = max(this->min_permeability[0], (this->children[i].permeability_3[j] - valuePermeability_3));
                    }
                }else if((j == 2) || (j == 3)){
                    if(tunning = 0){
                        this->children[i].porosity[j] = min(MAX_POROSITY, (this->children[i].porosity[j] + valuePorosity));
                        this->children[i].permeability_1[j] = min(this->max_permeability[1], (this->children[i].permeability_1[j] + valuePermeability_1));
                        this->children[i].permeability_2[j] = min(this->max_permeability[1], (this->children[i].permeability_2[j] + valuePermeability_2));
                        this->children[i].permeability_3[j] = min(this->max_permeability[1], (this->children[i].permeability_3[j] + valuePermeability_3));
                    }else{
                        this->children[i].porosity[j] = max(MIN_POROSITY, (this->children[i].porosity[j] - valuePorosity));
                        this->children[i].permeability_1[j] = max(this->min_permeability[1], (this->children[i].permeability_1[j] - valuePermeability_1));
                        this->children[i].permeability_2[j] = max(this->min_permeability[1], (this->children[i].permeability_2[j] - valuePermeability_2));
                        this->children[i].permeability_3[j] = max(this->min_permeability[1], (this->children[i].permeability_3[j] - valuePermeability_3));
                    }
                }
            }
        }else if(percent == 1){
            for(int j = 0; j < 5; j++){
                valuePorosity = ((this->children[i].porosity[j] * 10) / 100);
                valuePermeability_1 = ((this->children[i].permeability_1[j] * 10) / 100);
                valuePermeability_2 = ((this->children[i].permeability_2[j] * 10) / 100);
                valuePermeability_3 = ((this->children[i].permeability_2[j] * 10) / 100);
                if(j == 1){
                    break;
                }else if((j == 0) || (j == 4)){
                    if(tunning = 0){
                        this->children[i].porosity[j] = min(MAX_POROSITY, (this->children[i].porosity[j] + valuePorosity));
                        this->children[i].permeability_1[j] = min(this->max_permeability[0], (this->children[i].permeability_1[j] + valuePermeability_1));
                        this->children[i].permeability_2[j] = min(this->max_permeability[0], (this->children[i].permeability_2[j] + valuePermeability_2));
                        this->children[i].permeability_3[j] = min(this->max_permeability[0], (this->children[i].permeability_3[j] + valuePermeability_3));
                    }else{
                        this->children[i].porosity[j] = max(MIN_POROSITY, (this->children[i].porosity[j] + valuePorosity));
                        this->children[i].permeability_1[j] = max(this->min_permeability[0], (this->children[i].permeability_1[j] - valuePermeability_1));
                        this->children[i].permeability_2[j] = max(this->min_permeability[0], (this->children[i].permeability_2[j] - valuePermeability_2));
                        this->children[i].permeability_3[j] = max(this->min_permeability[0], (this->children[i].permeability_3[j] - valuePermeability_3));
                    }
                }else if((j == 2) || (j == 3)){
                    if(tunning = 0){
                        this->children[i].porosity[j] = min(MAX_POROSITY, (this->children[i].porosity[j] + valuePorosity));
                        this->children[i].permeability_1[j] = min(this->max_permeability[1], (this->children[i].permeability_1[j] + valuePermeability_1));
                        this->children[i].permeability_2[j] = min(this->max_permeability[1], (this->children[i].permeability_2[j] + valuePermeability_2));
                        this->children[i].permeability_3[j] = min(this->max_permeability[1], (this->children[i].permeability_3[j] + valuePermeability_3));
                    }else{
                        this->children[i].porosity[j] = max(MIN_POROSITY, (this->children[i].porosity[j] + valuePorosity));
                        this->children[i].permeability_1[j] = max(this->min_permeability[1], (this->children[i].permeability_1[j] - valuePermeability_1));
                        this->children[i].permeability_2[j] = max(this->min_permeability[1], (this->children[i].permeability_2[j] - valuePermeability_2));
                        this->children[i].permeability_3[j] = max(this->min_permeability[1], (this->children[i].permeability_3[j] - valuePermeability_3));
                    }
                }
            }
            
            
        }else if(percent == 2){
            for(int j = 0; j < 5; j++){
                valuePorosity = ((this->children[i].porosity[j] * 20) / 100);
                valuePermeability_1 = ((this->children[i].permeability_1[j] * 20) / 100);
                valuePermeability_2 = ((this->children[i].permeability_2[j] * 20) / 100);
                valuePermeability_3 = ((this->children[i].permeability_2[j] * 20) / 100);
                if(j == 1){
                    break;
                }else if((j == 0) || (j == 4)){
                    if(tunning = 0){
                        this->children[i].porosity[j] = min(MAX_POROSITY, (this->children[i].porosity[j] + valuePorosity));
                        this->children[i].permeability_1[j] = min(this->max_permeability[0], (this->children[i].permeability_1[j] + valuePermeability_1));
                        this->children[i].permeability_2[j] = min(this->max_permeability[0], (this->children[i].permeability_2[j] + valuePermeability_2));
                        this->children[i].permeability_3[j] = min(this->max_permeability[0], (this->children[i].permeability_3[j] + valuePermeability_3));
                    }else{
                        this->children[i].porosity[j] = max(MIN_POROSITY, (this->children[i].porosity[j] + valuePorosity));
                        this->children[i].permeability_1[j] = max(this->min_permeability[0], (this->children[i].permeability_1[j] - valuePermeability_1));
                        this->children[i].permeability_2[j] = max(this->min_permeability[0], (this->children[i].permeability_2[j] - valuePermeability_2));
                        this->children[i].permeability_3[j] = max(this->min_permeability[0], (this->children[i].permeability_3[j] - valuePermeability_3));
                    }
                }else if((j == 2) || (j == 3)){
                    if(tunning = 0){
                        this->children[i].porosity[j] = min(MAX_POROSITY, (this->children[i].porosity[j] + valuePorosity));
                        this->children[i].permeability_1[j] = min(this->max_permeability[1], (this->children[i].permeability_1[j] + valuePermeability_1));
                        this->children[i].permeability_2[j] = min(this->max_permeability[1], (this->children[i].permeability_2[j] + valuePermeability_2));
                        this->children[i].permeability_3[j] = min(this->max_permeability[1], (this->children[i].permeability_3[j] + valuePermeability_3));
                    }else{
                        this->children[i].porosity[j] = max(MIN_POROSITY, (this->children[i].porosity[j] + valuePorosity));
                        this->children[i].permeability_1[j] = max(this->min_permeability[1], (this->children[i].permeability_1[j] - valuePermeability_1));
                        this->children[i].permeability_2[j] = max(this->min_permeability[1], (this->children[i].permeability_2[j] - valuePermeability_2));
                        this->children[i].permeability_3[j] = max(this->min_permeability[1], (this->children[i].permeability_3[j] - valuePermeability_3));
                    }
                }
            }
        }
    }
}