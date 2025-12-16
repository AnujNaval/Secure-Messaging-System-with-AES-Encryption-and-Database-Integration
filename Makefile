# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g  # Add -g for debugging
LIBS = -lssl -lcrypto -lsqlite3

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIRS = /usr/include  # Add more paths if needed

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Targets
TARGETS = server client

# Default target: build both server and client
all: $(TARGETS)

# Rule for creating object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)  # Ensure build directory exists
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIRS) -c $< -o $@

# Rule for server target
server: $(BUILD_DIR)/server.o $(BUILD_DIR)/encryption.o $(BUILD_DIR)/database.o $(BUILD_DIR)/utils.o
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

# Rule for client target
client: $(BUILD_DIR)/client.o $(BUILD_DIR)/encryption.o $(BUILD_DIR)/utils.o
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

# Clean up build files
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGETS)

.PHONY: all clean
