# Compiler
CC = gcc

# Compiler and linker flags
CFLAGS = `pkg-config --cflags gtk+-3.0 vte-2.91` -Wall -Wextra -std=c11 -Iinclude -I/usr/local/include
LDFLAGS = `pkg-config --libs gtk+-3.0 vte-2.91` -L/usr/local/lib -lcjson

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
DEB_DIR = build/debian

# version
VERSION = 1.0-1

# Target executable
TARGET = $(BIN_DIR)/apxterm

# Source and object files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# Default target
all: $(TARGET)

# Link object files into final executable
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile C files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create object directory
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) *.deb

# Build Debian package
deb: $(TARGET)
	./build/build-deb.sh

.PHONY: all run clean deb