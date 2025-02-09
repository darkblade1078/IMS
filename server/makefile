# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Directories
SRC_DIR = src/code
HEADER_DIR = src/headers
OBJ_DIR = objects

# Find all source files and generate object file names
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Final executable name
TARGET = server.out

# Make sure the objects directory exists
$(shell mkdir -p $(OBJ_DIR))

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(HEADER_DIR) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all clean