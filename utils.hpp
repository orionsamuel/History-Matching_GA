struct individual{
    double porosity;
    double permeability_1;
    double permeability_2;
    double permeability_3;
    double error_rank;
};

struct result{
    double real_water;
    double real_oil;
};

#define SIZE_POPULATION 10
#define N_GENERATIONS 3

#define CROSSOVER_RATE 20

#define MUTATION_RATE 50

#define MIN_POROSITY 0.0000000E+00
#define MAX_POROSITY 0.3
//#define MAX_POROSITY 9.7229995E-02

#define MIN_PERMEABILITY 0.0000000E+00
#define MAX_PERMEABILITY 4.4235002E-02
