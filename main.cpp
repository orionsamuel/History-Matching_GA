#include <iostream>
#include "genetic.hpp"

int main(){
    genetic_algorithm ga;

    ga.init();

    cout << "Finish" << endl;

    ga.~genetic_algorithm();

    return 0;
}