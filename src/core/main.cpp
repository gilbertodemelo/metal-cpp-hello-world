#include <iostream>
#include <chrono>
#include "../include/vector_generator.hpp"



int main() {

    int n = 1000;
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

    MTL::Device *device = MTL::CreateSystemDefaultDevice();
    if (!device) {
        std::cerr << "Metal is not support on this device.\n";
    }

    auto t3 = std::chrono::high_resolution_clock::now();
    add_vectors_gpu(device, a, b, c, n);
    auto t4 = std::chrono::high_resolution_clock::now();
    std::cout << "GPU: " << std::chrono::duration<double>(t4 - t1).count() << "s\n";

    device->release();


    


    return 0;
}