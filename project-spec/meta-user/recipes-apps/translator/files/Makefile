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
APP_OBJS := $(SRC_FILES:.cpp=.o)
APP_OBJS := $(APP_OBJS:.c=.o)

# Compile .c files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CXX) $(CFLAGS) -c $< -o $@
	@echo

$(APP): $(APP_OBJS)
	$(CXX) -o $@ $(APP_OBJS) $(LDFLAGS) $(LDLIBS)
	@echo

# --- Special Commands ---

.PHONY: all build clean

all: build

build: $(APP)

clean:
	rm -f $(APP) $(APP_OBJS)
