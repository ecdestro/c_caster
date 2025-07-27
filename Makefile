# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I/usr/include/SDL2
LDFLAGS = -lSDL2 -lSDL2_image -lm

# Project structure
SRC_DIR = src
BUILD_DIR = build
TARGET = c_caster

# Files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Default target
all: $(BUILD_DIR)/$(TARGET)

# Linking
$(BUILD_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Build complete: $@"

# Compilation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Build directory cleaned."

# Phony targets
.PHONY: all clean
