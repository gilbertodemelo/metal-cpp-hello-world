#ifndef VECTOR_GENERATOR_HPP
#define VECTOR_GENERATOR_HPP

#include <random>
#include <type_traits>

template <typename T>
T *generate_vector(int size, T min, T max) {
     std::random_device rd;
     std::mt19937 generator(rd());

     T* arr = new T[size];
     
     if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> distribution(min, max);
        for(int i = 0; i < size; i++) {
            arr[i] = distribution(generator);
        }
     } else if constexpr(std::is_floating_point_v<T>) {
        std::uniform_real_distribution<T> distribution(min, max);
        for(int i = 0; i < size; i++) {
            arr[i] = distribution(generator);
        }
     }

     return arr;
}

#endif // VECTOR_GENERATOR_HPP