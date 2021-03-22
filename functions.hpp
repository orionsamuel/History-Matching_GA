#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <random>
#include <iomanip>
#include <vector>
#include "utils.hpp"

using namespace std;

class functions{
    public:

    void simulation(int idIteration);
    float rand(float min, float max);
    void sort_rank();
    const vector<string> split(const string& s, const char& c);

};

