# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Directory paths
SRCDIR = code
HEADERDIR = headers
OBJDIR = objects

# Find all source and header files
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
HEADERS = $(wildcard $(HEADERDIR)/*.h)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Main target
TARGET = server.out

# Create object directory if it doesn't exist
$(shell mkdir -p $(OBJDIR))

# Main target rule
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Rule for object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -I$(HEADERDIR) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJDIR)/*.o $(TARGET)

.PHONY: clean