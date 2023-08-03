# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Werror

# Linker flags
LDFLAGS = -lcurses

# Source and object file directories
SRC_DIR = ./src
OBJ_DIR = ./obj

# Source files (excluding main.cpp)
SRC_FILES = $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))

# Object files for all source files (excluding main.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Output binary name
OUTPUT_BINARY = dungeon_monsters

# Targets
all: $(OUTPUT_BINARY)

$(OUTPUT_BINARY): $(OBJ_FILES) $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_FILES) $(SRC_DIR)/main.cpp $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(OUTPUT_BINARY)