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
#define N_GENERATIONS 50

#define CROSSOVER_RATE 20
#define MUTATION_RATE 50

#define MIN_POROSITY 1.0452609E-01
#define MAX_POROSITY 0.3

#define MIN_PERMEABILITY 1.000000E-12
#define MAX_PERMEABILITY 2.423500E-14

#define WATER_WEIGHT 0.6
#define OIL_WEIGHT 0.5

#define N_METRICS 2
