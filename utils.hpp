struct individual{
    double porosity;
    double permeability_1;
    double permeability_2;
    double permeability_3;
    double error_rank;
};

struct result{
    double water;
    double oil;
};

#define SIZE_POPULATION 100
#define N_GENERATIONS 3

#define CROSSOVER_RATE 20
#define MUTATION_RATE 50

#define MIN_POROSITY 0.0000000E+00
#define MAX_POROSITY 0.1

#define MIN_PERMEABILITY 0.0000000E+00
#define MAX_PERMEABILITY 2.4235002E-02

#define WATER_WEIGHT 0.5
#define OIL_WEIGHT 0.5

#define N_METRICS 2