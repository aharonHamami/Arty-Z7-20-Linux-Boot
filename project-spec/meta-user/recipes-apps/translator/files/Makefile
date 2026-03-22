APP = translator

# --- Compiler ---
CXX ?= g++
LDFLAGS ?= 
LDLIBS ?= 

# --- Directories ---
SRC_DIR := ./src
OBJ_DIR := ./obj

# --- Files ---
SRC_FILES = $(shell find $(SRC_DIR) -type f -name '*.cpp' -o -name '*.c')
APP_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Compile .c files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D) # Create the object file's directory if it doesn't exist
	$(CXX) $(CFLAGS) -c $< -o $@
	@echo

$(APP): $(APP_OBJS)
	$(CXX) -o $@ $(APP_OBJS) $(LDFLAGS) $(LDLIBS)
	@echo

# --- Special Commands ---

.PHONY: all build clean debug

all: build

build: $(APP)

debug: CFLAGS += -g -O0
debug: build
	#$(GDB) $(APP)
	#petalinux-util gdb $(APP)

clean:
	rm -f $(APP) $(APP_OBJS)
	rm -rf ./$(OBJ_DIR)
