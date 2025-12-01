#include <iostream>
#include "../include/vector_generator.hpp"



int main() {

    int n = 100;
    int min= 0;
    int max = 10000;

    int *a = generate_vector(n, min, max);



    return 0;
}