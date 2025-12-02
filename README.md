# Metal C++ Vector Addition

This project demonstrates how to perform vector addition using Apple's Metal API with C++. It compares the performance of CPU-based execution (commented out by default) versus GPU-based execution.

## Project Structure

- `src/core/main.cpp`: Main entry point. Generates random vectors and triggers the GPU computation.
- `src/core/VectorAddition.metal`: The Metal shader kernel that performs the vector addition on the GPU.
- `src/include/vector_generator.hpp`: Contains helper functions for vector generation and the host-side Metal implementation (`add_vectors_gpu`).
- `Makefile`: Build script to compile the C++ code and Metal shaders.

## Prerequisites

- macOS with a Metal-capable GPU (Apple Silicon M1/M2/M3 recommended for unified memory benefits).
- Xcode Command Line Tools installed (`xcode-select --install`).
- `clang++` and `xcrun`.

## How to Build

To build the project, run:

```bash
make
```

This will create a `build` directory containing the `vecadd` executable and the `default.metallib` library.

## How to Run

To run the program:

```bash
make run
```

Or execute the binary directly:

```bash
./build/vecadd
```

## Important Note on Memory Usage

The default vector size in `main.cpp` is set to **10 billion elements** (`10'000'000'000`).
- Each vector is `float` (4 bytes).
- 3 vectors (A, B, C) are allocated.
- Total memory required: ~120 GB.

**Warning:** This will likely exceed the available RAM on most systems, leading to a crash or extreme swapping. If you encounter memory issues, reduce the `n` variable in `src/core/main.cpp` to a smaller value (e.g., `100'000'000` for ~1.2 GB).
