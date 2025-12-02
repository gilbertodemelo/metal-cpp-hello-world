#include <iostream>
#include <chrono>
#include "../include/vector_generator.hpp"



int main() {

    size_t n = 10'000'000;
    float min= 0.0f;
    float max = 10.0f * (float) n;

    float *a = generate_vector(n, min, max);
    float *b = generate_vector(n, min, max);
    float *c = new float[n];


    //--- CPU
    auto t1 = std::chrono::high_resolution_clock::now();
    add_vectors_cpu(a, b, c, n);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "CPU: " << std::chrono::duration<double>(t2 - t1).count() << "s\n";

    //--- GPU

    auto device = MTL::CreateSystemDefaultDevice();
    if (!device) {
        std::cerr << "Metal is not support on this device.\n";
        delete[] a; delete[] b; delete[] c;
    }

    auto t3 = std::chrono::high_resolution_clock::now();
    add_vectors_gpu(device, a, b, c, n);
    auto t4 = std::chrono::high_resolution_clock::now();
    std::cout << "GPU: " << std::chrono::duration<double>(t4 - t3).count() << "s\n";

    // --- Cleanup
    delete[] a;
    delete[] b;
    delete[] c;

    return 0;
}