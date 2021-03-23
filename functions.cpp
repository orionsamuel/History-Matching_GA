#include "functions.hpp"

void functions::simulation(int idIteration){
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

double functions::rand_double(double min, double max){
    random_device rd;
    default_random_engine eng(rd());
    uniform_real_distribution<double>distr(min, max);

    double num = distr(eng);

    return num;
}

void functions::sort_rank(){

}

double functions::max(double num1, double num2){
    if(num1 > num2){
        return num1;
    }else{
        return num2;
    }
}

double functions::min(double num1, double num2){
    if(num1 < num2){
        return num1;
    }else{
        return num2;
    }
}

const vector<string> functions::split(const string& s, const char& c){
    string buff{""};
	vector<string> v;
	
	for(auto n:s){
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}

	if(buff != "") v.push_back(buff);
	
	return v;
}