#include <iostream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <random>
#include <iomanip>
#include <vector>
#include <fstream>
#include "utils.hpp"

using namespace std;

class functions{
    public:

    void simulation(int idIteration);
    double rand_double(double min, double max);
    void sort_rank();
    double max(double num1, double num2);
    double min(double num1, double num2);
    void getReal_results(result* values);
    const vector<string> split(const string& s, const char& c);

};

