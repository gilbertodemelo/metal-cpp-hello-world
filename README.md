# Metal C++ Vector Addition

This project demonstrates how to perform vector addition using Apple's Metal API with C++. It compares the performance of CPU-based execution (commented out by default) versus GPU-based execution.

## Project Structure

- `src/core/main.cpp`: Main entry point. Generates random vectors and triggers the GPU computation.
- `src/core/VectorAddition.metal`: The Metal shader kernel that performs the vector addition on the GPU.
- `src/include/vector_generator.hpp`: Contains helper functions for vector generation and the host-side Metal implementation (`add_vectors_gpu`).
- `Makefile`: Build script to compile the C++ code and Metal shaders.

## Running Metal Outside Xcode

Unlike traditional iOS/macOS development that relies heavily on Xcode, this project demonstrates how to use Metal directly from the command line using C++.

**Key Differences:**
- **No Xcode Project:** There is no `.xcodeproj` or `.xcworkspace` file. The build process is managed entirely by a `Makefile`.
- **Command Line Tools:** We use `clang++` for compiling C++ code and `xcrun` to invoke Metal tools (`metal` for compiling shaders to AIR, `metallib` for creating the library).
- **Metal-cpp:** This project utilizes `metal-cpp`, a C++ interface for Metal, allowing us to write Metal applications in standard C++ without Objective-C/Swift.

This approach offers a lightweight, transparent way to understand the Metal compilation pipeline and is ideal for cross-platform C++ developers or those integrating Metal into custom build systems.

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


