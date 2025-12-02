#ifndef VECTOR_GENERATOR_HPP
#define VECTOR_GENERATOR_HPP

#include <iostream>
#include <random>
#include <type_traits>
#include <cstring>
#include <cassert>


#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

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

template <typename T>
void add_vectors_cpu(const T *A, const  T *B, T *C, int size) {
    for (int i = 0; i < size; i++) {
        C[i] = A[i] + B[i];
    }
}


void add_vectors_gpu(MTL::Device *device, const float *A, const float *B, float *C, int N) {

    // 1. Guarantee that the device (GPU) is valid
    assert(device);

    // 2. Create a command queue to send instructions to the GPU
    MTL::CommandQueue *commandQueue = device->newCommandQueue();

    NS::Error *error = nullptr;

    // 3. Loads the standard library with functions written in Metal Shading Language (MSL)
    MTL::Library *library = device->newDefaultLibrary();
    
    if (!library) {
        std::cerr << "Failed to load default.metallib library." << std::endl;
        exit(1);
    }

    // 4. Takes the function "vectorAddShader" written in Metal (inside the .metal shader file)
    MTL::Function *func = library->newFunction(NS::String::string("vectorAddShader", NS::UTF8StringEncoding));

    // 5. Creates the computing pipeline with the shader function
    MTL::ComputePipelineState *pipelineState = device->newComputePipelineState(func, &error);

    if (!pipelineState) {
        std::cerr << "Failed to create pipeline state: " << NS::String::string(error->localizedDescription()->utf8String(), NS::UTF8StringEncoding)->utf8String() << std::endl;
        exit(1);
    }

    // 6. Calculate the size of the necessary buffers (in bytes)
    size_t sizeA = sizeof(float) * N;
    size_t sizeB = sizeof(float) * N;
    size_t sizeC = sizeof(float) * N;

    // 7. Allocated the memory buffers in the GPU
    // Note that that this mode is only efficient in Apple Silicon (M1+)
    MTL::Buffer *bufA = device->newBuffer(sizeA, MTL::ResourceStorageModeShared);
    MTL::Buffer *bufB = device->newBuffer(sizeB, MTL::ResourceStorageModeShared);
    MTL::Buffer *bufC = device->newBuffer(sizeC, MTL::ResourceStorageModeShared);

    // 8. Copy the data from vectors A and B from the host (CPU) to the device (GPU) buffers
    memcpy(bufA->contents(), A, sizeA);
    memcpy(bufB->contents(), B, sizeB);

    // 9. Create a new command buffer
    MTL::CommandBuffer *commandBuffer = commandQueue->commandBuffer();

    // 10. Create an encoder to encode compute commands
    MTL::ComputeCommandEncoder *encoder = commandBuffer->computeCommandEncoder();

    // 11. Set up the pipeline state (binds the shader to execution)
    encoder->setComputePipelineState(pipelineState);

    // 12. Define the input and output buffers for the shader
    encoder->setBuffer(bufA, 0, 0); // vector A in index 0
    encoder->setBuffer(bufB, 0, 1); // vector B in index 1
    encoder->setBuffer(bufC, 0, 2); // vector C (results) in index 2

    // 13. Pass the size of the vectors as simple (scalar) values to the shader
    encoder->setBytes(&N, sizeof(uint), 3);

    // 14. Define the execution grid (how many threads will run in the GPU)
    // Here, we are asking for N threads (one for each element in results vector C)
    MTL::Size gridSize = MTL::Size(N, 1, 1);

    // 15. Size of each group (block) of threads
    // Always use a multiple of 32.
    MTL::Size threadgroupSize = MTL::Size(256, 1, 1);

    // 16. Send the job to the device
    encoder->dispatchThreads(gridSize, threadgroupSize);

    // 17. Finalize the encoding of commands
    encoder->endEncoding();

    // 18. Sends the command buffer for execution
    commandBuffer->commit();

    // 19. Wait for the execution to terminate (sincronization with the CPU)
    commandBuffer->waitUntilCompleted();

    // 20. Copy the results of vector C back from the CPU from the GPU
    memcpy(C, bufC->contents(), sizeC);

    // 21. Free the resources
    bufA->release();
    bufB->release();
    bufC->release();
    pipelineState->release();
    func->release();
    library->release();
    commandQueue->release();
    commandBuffer->release();

}

#endif // VECTOR_GENERATOR_HPP