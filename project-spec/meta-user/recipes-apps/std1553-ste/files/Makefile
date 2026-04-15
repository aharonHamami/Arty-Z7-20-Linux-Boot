APP = std1553-ste

# --- Compiler ---
CC ?= gcc
CFLAGS := -Iinclude -Wall -Wextra -std=gnu11
LDFLAGS ?=  

# --- Directories ---
SRC_DIR := src/
OBJ_DIR := build/obj
BIN_DIR := build/bin

# --- Files ---
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Link object files to create an executable
$(APP): $(OBJ_FILES)
	@echo "Linking object files: $^"
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rules to automatically create directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# --- Special Commands ---

.PHONY: all debug release run clean help

all: $(OBJ_DIR) $(BIN_DIR)

# Build with extra debugging flags
debug: CXXFLAGS += -g -O0 -DDEBUG
debug: clean all

# Release build with optimizations
release: CXXFLAGS += -O3 -DNDEBUG
release: clean all

# Run the program
run: all
	@echo Executing program: $(APP)
	@echo
	@$(APP)

clean:
	rm -rf ./$(APP) ./$(OBJ_DIR) ./$(BIN_DIR)

help:
	@echo "Available targets:"
	@echo "  all       - Build the main executable"
	@echo "  debug     - Build with debug flags"
	@echo "  release   - Build with optimizations"
	@echo "  run       - Build and run the executable"
	@echo "  clean     - Remove build artifacts"
	@echo "  help      - Show this help"
