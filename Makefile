# Compiler and flags
CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall -Wextra -Wpedantic -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

# Directories
SRC_DIR = src
BIN_DIR = bin

# Find all .cpp files in src/ (recursively)
SRCS = $(shell find $(SRC_DIR) -type f \( -name "*.cpp" -o -name "*.c" \) )

# Separate main.cpp from the rest
MAIN_SRC = $(filter $(SRC_DIR)/main.cpp,$(SRCS))
OTHER_SRCS = $(filter-out $(SRC_DIR)/main.cpp,$(SRCS))

# Executable name
TARGET = $(BIN_DIR)/main

# Default target: build the program
all: $(TARGET)

# Link and compile all .cpp files to create the executable in one step
$(TARGET): $(MAIN_SRC) $(OTHER_SRCS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) $(OTHER_SRCS) -o $(TARGET)

# Clean up build artifacts
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
