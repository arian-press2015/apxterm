# Compiler
CC = gcc

# Compiler and linker flags
CFLAGS = `pkg-config --cflags gtk+-3.0 vte-2.91` -Wall -Wextra -std=c11 -Iinclude -I/usr/local/include
LDFLAGS = `pkg-config --libs gtk+-3.0 vte-2.91` -L/usr/local/lib -lcjson

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
DEB_DIR = debian

# Target executable
TARGET = $(BIN_DIR)/apXterm

# Source and object files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

DEB_PACKAGE = apxterm_1.0.1_amd64.deb

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
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Build Debian package
deb: $(TARGET)
	# Create Debian package structure
	@mkdir -p $(DEB_DIR)/usr/bin
	@mkdir -p $(DEB_DIR)/usr/share/apxterm
	@mkdir -p $(DEB_DIR)/DEBIAN
	
	# Copy the binary to /usr/bin in the package
	cp $(TARGET) $(DEB_DIR)/usr/bin/
	cp config.json $(DEB_DIR)/usr/share/apxterm/config.json
	
	# Create the control file
	@echo "Package: apxterm" > $(DEB_DIR)/DEBIAN/control
	@echo "Version: 1.0.0" >> $(DEB_DIR)/DEBIAN/control
	@echo "Section: utils" >> $(DEB_DIR)/DEBIAN/control
	@echo "Priority: optional" >> $(DEB_DIR)/DEBIAN/control
	@echo "Architecture: amd64" >> $(DEB_DIR)/DEBIAN/control
	@echo "Depends: libgtk-3-0 (>= 3.0), libvte-2.91-0 (>= 0.50), libcjson1" >> $(DEB_DIR)/DEBIAN/control
	@echo "Maintainer: AP2015 <arian.press2015@gmail.com>" >> $(DEB_DIR)/DEBIAN/control
	@echo "Description: A MobaXterm for Linux" >> $(DEB_DIR)/DEBIAN/control
	@echo " apXterm is a GTK+-based terminal emulator with support for multiple SSH sessions." >> $(DEB_DIR)/DEBIAN/control
	
	# Create postinst script
	@echo "#!/bin/sh" > $(DEB_DIR)/DEBIAN/postinst
	@echo "set -e" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "# Create ~/.config/apxterm if it doesn't exist" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "[ ! -d \"\$$HOME/.config/apxterm\" ] && mkdir -p \"\$$HOME/.config/apxterm\"" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "# Copy default config if not present" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "[ ! -f \"\$$HOME/.config/apxterm/config.json\" ] && cp /usr/share/apxterm/config.json \"\$$HOME/.config/apxterm/config.json\"" >> $(DEB_DIR)/DEBIAN/postinst
	@chmod 755 $(DEB_DIR)/DEBIAN/postinst
	@chmod 755 $(DEB_DIR)/usr/bin/apXterm
	@chmod 644 $(DEB_DIR)/usr/share/apxterm/config.json
	dpkg-deb --build $(DEB_DIR) $(DEB_PACKAGE)

	rm -r debian

.PHONY: all run clean deb