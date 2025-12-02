# Paths
SRC_DIR := src
BUILD_DIR := build
METALCPP_DIR := external/metal-cpp

# Filenames
CPP_SOURCES := $(SRC_DIR)/core/main.cpp
METAL_SOURCE := $(SRC_DIR)/core/VectorAddition.metal
METAL_AIR := $(BUILD_DIR)/MatrixMul.air
METALLIB := $(BUILD_DIR)/default.metallib
EXECUTABLE := $(BUILD_DIR)/vecadd

# Compiler and tools
CXX := clang++
METALC := xcrun -sdk macosx metal
METALLIB_TOOL := xcrun -sdk macosx metallib

CXXFLAGS := -std=c++17 -stdlib=libc++ -I $(METALCPP_DIR) -fno-objc-arc -O2 -fopenmp
LDFLAGS := -framework Metal -framework Foundation -framework QuartzCore -fopenmp


# Targets
.PHONY: all clean run

all: $(EXECUTABLE)

# Compile Metal shader to AIR
$(METAL_AIR): $(METAL_SOURCE) | $(BUILD_DIR)
	$(METALC) -c $< -o $@

# Link AIR to metallib
$(METALLIB): $(METAL_AIR)
	$(METALLIB_TOOL) $< -o $@

# Compile C++ and link
$(EXECUTABLE): $(CPP_SOURCES) $(METALLIB) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# Create build directory if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Run program
run: all
	@echo "Running program..."
	@$(EXECUTABLE)
