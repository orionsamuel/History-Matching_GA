#include "genetic.hpp"

genetic_algorithm::genetic_algorithm(){
    srand (time(0));
}

genetic_algorithm::~genetic_algorithm(){

}

void genetic_algorithm::init(){    
    system("rm -r -f Output/*");

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
        this->real_results[i].water = stod(v[i]);
        this->real_results[i].oil = stod(v2[i]);
    }

    readDataset();
    int count = 1;
    while(this->population[0].error_rank > 2.000000E-06){
        createPopulations(count);
        count++;
    }
}

void genetic_algorithm::readDataset(){
    DIR* dp = opendir("Output/0");

    if(dp == NULL){
        system("mkdir Output/0");
    }else{
        system("rm -f Output/0/*");
    }

    vector<individual> dataset;

    //Leitura do dataset e criação do arquivo da população 0
    ifstream read_input("Dataset/history_matching.csv", ios::in);
    ofstream write_output("Output/0/0.csv", ios::out);

    string line;

    //Passando os valores contidos no arquivo para um vector do tipo individuo
    int count = 0;
    while(!read_input.eof()){
        getline(read_input, line);
        if(count == 0){
            write_output << line << endl;
        }else if(count < 4501){
            vector<string> v{split(line, ',')};
            dataset.push_back({stod(v[0]), stod(v[1]), stod(v[2]), stod(v[3]), stod(v[4])});
        }
        count++;        
    }

    read_input.close();

    sort(begin(dataset), end(dataset), compare);

    for(int i = 0; i < SIZE_POPULATION; i++){
        write_output << scientific << dataset[i].porosity << "," << dataset[i].permeability_1 << "," << dataset[i].permeability_2 << "," << dataset[i].permeability_3 << "," << dataset[i].error_rank << endl;
    }

    write_output.close();

    ifstream read_csv("Output/0/0.csv", ios::in);

    string decimal;
    line = "";
    
    //Pega a casa decimal da permeabilidade da menor taxa de erro, para gerar os valores mínimos e máximos de permeabilidade
    count = 0;
    while(!read_csv.eof()){
        getline(read_csv, line);
        if(count == 1){
            vector<string> v{split(line, ',')};
            decimal = v[1][10];
            decimal += v[1][11];
            cout << endl;
        }

        count++;        
    }

    read_csv.close();
    
    string minPermeability = "1.000000e-"+decimal;
    string maxPermeability = "4.000000e-"+decimal;

    this->min_permeability = stod(minPermeability);
    this->max_permeability = stod(maxPermeability);

    system("rm Output/0/0.csv");

    for(int i = 0; i < SIZE_POPULATION; i++){
        string command = "cp Input/inputDS.dat Output/0/inputDS_"+to_string(i)+".dat";
        const char* file = (char*) command.c_str();
        system(file);
    }

    for(int i = 0; i < SIZE_POPULATION; i++){
        this->population[i].porosity = rand_double(MIN_POROSITY, MAX_POROSITY);
        this->population[i].permeability_1 = rand_double(this->min_permeability, this->max_permeability);
        this->population[i].permeability_2 = rand_double(this->min_permeability, this->max_permeability);
        this->population[i].permeability_3 = rand_double(this->min_permeability, this->max_permeability);
    }

    for(int i = 0; i < SIZE_POPULATION; i++){
        ifstream read_input("Output/0/inputDS_"+to_string(i)+".dat", ios::in);
        ofstream write_input("inputDS_"+to_string(i)+".dat", ios::out);
        int count = 0;
        line = "";
        while(!read_input.eof()){
            getline(read_input, line);
            if(count == 142){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity << "   " << this->population[i].permeability_1 << "   " << this->population[i].permeability_2 << "   " << this->population[i].permeability_3 << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
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
    sort(begin(this->population), end(this->population), compare);

    for(int i = 0; i < SIZE_POPULATION; i++){
        ifstream read_input("Output/0/inputDS_"+to_string(i)+".dat", ios::in);
        ofstream write_input("inputDS_"+to_string(i)+".dat", ios::out);
        int count = 0;
        line = "";
        while(!read_input.eof()){
            getline(read_input, line);
            if(count == 142){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity << "   " << this->population[i].permeability_1 << "   " << this->population[i].permeability_2 << "   " << this->population[i].permeability_3 << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
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
}

void genetic_algorithm::createPopulations(int idIteration){
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
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity << "   " << this->population[i].permeability_1 << "   " << this->population[i].permeability_2 << "   " << this->population[i].permeability_3 << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
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
    sort(begin(this->population), end(this->population), compare);

    for(int i = 0; i < SIZE_POPULATION; i++){
        ifstream read_input("Output/"+to_string(idIteration)+"/inputDS_"+to_string(i)+".dat", ios::in);
        ofstream write_input("inputDS_"+to_string(i)+".dat", ios::out);
        int count = 0;
        string line;
        while(!read_input.eof()){
            getline(read_input, line);
            if(count == 142){
                vector<string> v{split(line, ' ')};
                write_input << "         " << v[0] << "   " << scientific << this->population[i].porosity << "   " << this->population[i].permeability_1 << "   " << this->population[i].permeability_2 << "   " << this->population[i].permeability_3 << "   " << v[5] << "   "  << v[6] << "   " << v[7] << "   " << v[8] << "   " << v[9] << endl;
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
}

void genetic_algorithm::fitness(int idIteration){
    ofstream write_error("Output/"+to_string(idIteration)+"/error.txt", ios::out);

    for(int i = 0; i < SIZE_POPULATION; i++){
        double rank;

        ifstream result_water("Output/"+to_string(idIteration)+"/vazaoAgua_"+to_string(i)+".dat", ios::in);
        ifstream result_oil("Output/"+to_string(idIteration)+"/vazaoOleo_"+to_string(i)+".dat", ios::in);

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

        write_error << scientific << rank << endl;

    }

    write_error.close();
}

void genetic_algorithm::crossover(){
    for(int i = 0; i < crossover_rate; i++){
        this->children[i].porosity = 0;
        this->children[i].permeability_1 = 0;
        this->children[i].permeability_2 = 0;
        this->children[i].permeability_3 = 0;
    }

    //Escolhe o ponto de corte
    int percent = rand() % 3;
    if(percent == 0){
        int count = 0;
        while(count < crossover_rate){
            this->children[count].porosity = this->population[count].porosity;
            this->children[count].permeability_1 = this->population[count + 1].permeability_1;
            this->children[count].permeability_2 = this->population[count + 1].permeability_2;
            this->children[count].permeability_3 = this->population[count + 1].permeability_3;

            this->children[count + 1].porosity = this->population[count + 1].porosity;
            this->children[count + 1].permeability_1 = this->population[count].permeability_1;
            this->children[count + 1].permeability_2 = this->population[count].permeability_2;
            this->children[count + 1].permeability_3 = this->population[count].permeability_3;

            count = count + 2;
        }
    }else if(percent == 1){
        int count = 0;
        while(count < crossover_rate){
            this->children[count].porosity = this->population[count].porosity;
            this->children[count].permeability_1 = this->population[count].permeability_1;
            this->children[count].permeability_2 = this->population[count + 1].permeability_2;
            this->children[count].permeability_3 = this->population[count + 1].permeability_3;

            this->children[count + 1].porosity = this->population[count + 1].porosity;
            this->children[count + 1].permeability_1 = this->population[count + 1].permeability_1;
            this->children[count + 1].permeability_2 = this->population[count].permeability_2;
            this->children[count + 1].permeability_3 = this->population[count].permeability_3;

            count = count + 2;
        }
    }else if(percent == 2){
        int count = 0;
        while(count < crossover_rate){
            this->children[count].porosity = this->population[count].porosity;
            this->children[count].permeability_1 = this->population[count].permeability_1;
            this->children[count].permeability_2 = this->population[count].permeability_2;
            this->children[count].permeability_3 = this->population[count + 1].permeability_3;

            this->children[count + 1].porosity = this->population[count + 1].porosity;
            this->children[count + 1].permeability_1 = this->population[count + 1].permeability_1;
            this->children[count + 1].permeability_2 = this->population[count + 1].permeability_2;
            this->children[count + 1].permeability_3 = this->population[count].permeability_3;

            count = count + 2;
        }
    }

    mutation();

    int count = 0;
    for(int i = (SIZE_POPULATION - crossover_rate); i < SIZE_POPULATION; i++){
        this->population[i].porosity = this->children[count].porosity;
        this->population[i].permeability_1 = this->children[count].permeability_1;
        this->population[i].permeability_2 = this->children[count].permeability_2;
        this->population[i].permeability_3 = this->children[count].permeability_3;

        count++;
    }
    
}

void genetic_algorithm::mutation(){
     for(int i = 0; i < mutation_rate; i++){
        int percent = rand() % 3;
        int tunning = rand() % 2;

        if(percent == 0){
            double valuePorosity = ((this->children[i].porosity * 5) / 100);
            double valuePermeability_1 = ((this->children[i].permeability_1 * 5) / 100);
            double valuePermeability_2 = ((this->children[i].permeability_2 * 5) / 100);
            double valuePermeability_3 = ((this->children[i].permeability_2 * 5) / 100);
            if(tunning = 0){
                this->children[i].porosity = min(MAX_POROSITY, (this->children[i].porosity + valuePorosity));
                this->children[i].permeability_1 = min(this->max_permeability, (this->children[i].permeability_1 + valuePermeability_1));
                this->children[i].permeability_2 = min(this->max_permeability, (this->children[i].permeability_2 + valuePermeability_2));
                this->children[i].permeability_3 = min(this->max_permeability, (this->children[i].permeability_3 + valuePermeability_3));
            }else{
                this->children[i].porosity = max(MIN_POROSITY, (this->children[i].porosity - valuePorosity));
                this->children[i].permeability_1 = max(this->min_permeability, (this->children[i].permeability_1 - valuePermeability_1));
                this->children[i].permeability_2 = max(this->min_permeability, (this->children[i].permeability_2 - valuePermeability_2));
                this->children[i].permeability_3 = max(this->min_permeability, (this->children[i].permeability_3 - valuePermeability_3));
            }
        }else if(percent == 1){
            double valuePorosity = ((this->children[i].porosity * 10) / 100);
            double valuePermeability_1 = ((this->children[i].permeability_1 * 10) / 100);
            double valuePermeability_2 = ((this->children[i].permeability_2 * 10) / 100);
            double valuePermeability_3 = ((this->children[i].permeability_2 * 10) / 100);
            if(tunning = 0){
                this->children[i].porosity = min(MAX_POROSITY, (this->children[i].porosity + valuePorosity));
                this->children[i].permeability_1 = min(this->max_permeability, (this->children[i].permeability_1 + valuePermeability_1));
                this->children[i].permeability_2 = min(this->max_permeability, (this->children[i].permeability_2 + valuePermeability_2));
                this->children[i].permeability_3 = min(this->max_permeability, (this->children[i].permeability_3 + valuePermeability_3));
            }else{
                this->children[i].porosity = max(MIN_POROSITY, (this->children[i].porosity + valuePorosity));
                this->children[i].permeability_1 = max(this->min_permeability, (this->children[i].permeability_1 - valuePermeability_1));
                this->children[i].permeability_2 = max(this->min_permeability, (this->children[i].permeability_2 - valuePermeability_2));
                this->children[i].permeability_3 = max(this->min_permeability, (this->children[i].permeability_3 - valuePermeability_3));
            }
        }else if(percent == 2){
            double valuePorosity = ((this->children[i].porosity * 20) / 100);
            double valuePermeability_1 = ((this->children[i].permeability_1 * 20) / 100);
            double valuePermeability_2 = ((this->children[i].permeability_2 * 20) / 100);
            double valuePermeability_3 = ((this->children[i].permeability_2 * 20) / 100);
            if(tunning = 0){
                this->children[i].porosity = min(MAX_POROSITY, (this->children[i].porosity + valuePorosity));
                this->children[i].permeability_1 = min(this->max_permeability, (this->children[i].permeability_1 + valuePermeability_1));
                this->children[i].permeability_2 = min(this->max_permeability, (this->children[i].permeability_2 + valuePermeability_2));
                this->children[i].permeability_3 = min(this->max_permeability, (this->children[i].permeability_3 + valuePermeability_3));
            }else{
                this->children[i].porosity = max(MIN_POROSITY, (this->children[i].porosity + valuePorosity));
                this->children[i].permeability_1 = max(this->min_permeability, (this->children[i].permeability_1 - valuePermeability_1));
                this->children[i].permeability_2 = max(this->min_permeability, (this->children[i].permeability_2 - valuePermeability_2));
                this->children[i].permeability_3 = max(this->min_permeability, (this->children[i].permeability_3 - valuePermeability_3));
            }
        }
    }
}