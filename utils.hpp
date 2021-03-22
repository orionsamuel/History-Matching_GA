struct individual{
    float porosity;
    float permeability_1;
    float permeability_2;
    float permeability_3;
    float error_rank;
};

#define SIZE_POPULATION 10
#define N_GENERATIONS 50

#define MIN_POROSITY 0.0000000E+00
#define MAX_POROSITY 0.3
//#define MAX_POROSITY 9.7229995E-02

#define MIN_PERMEABILITY 0.0000000E+00
#define MAX_PERMEABILITY 4.4235002E-02
