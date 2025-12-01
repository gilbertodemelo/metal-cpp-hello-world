#include <iostream>
#include "../include/vector_generator.hpp"



int main() {

    int n = 10;
    int min= 0;
    int max = 10;

    int *a = generate_vector(n, min, max);
    int *b = generate_vector(n, min, max);
    int *c = new int[n];

    add_vectors(a, b, c, n);
    

    // add vectors
    for (int i = 0; i < n; i++) {
        std::cout << a[i] << " + " << b[i] << " = " << c[i] << std::endl;
    }


    return 0;
}