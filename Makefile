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

# version
VERSION = 1.0-1

# Target executable
TARGET = $(BIN_DIR)/apxterm

# Source and object files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

DEB_PACKAGE = apxterm_$(VERSION)_amd64.deb

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
	@echo "Version: $(VERSION)" >> $(DEB_DIR)/DEBIAN/control
	@echo "Section: utils" >> $(DEB_DIR)/DEBIAN/control
	@echo "Priority: optional" >> $(DEB_DIR)/DEBIAN/control
	@echo "Architecture: amd64" >> $(DEB_DIR)/DEBIAN/control
	@echo "Depends: libgtk-3-0 (>= 3.0), libvte-2.91-0 (>= 0.50), libcjson1" >> $(DEB_DIR)/DEBIAN/control
	@echo "Maintainer: AP2015 <arian.press2015@gmail.com>" >> $(DEB_DIR)/DEBIAN/control
	@echo "Description: A MobaXterm for Linux" >> $(DEB_DIR)/DEBIAN/control
	@echo " apxterm is a GTK+-based terminal emulator with support for multiple SSH sessions." >> $(DEB_DIR)/DEBIAN/control
	
	# Create postinst script
	@echo "#!/bin/sh" > $(DEB_DIR)/DEBIAN/postinst
	@echo "set -e" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "LOG=\"/tmp/apxterm_postinst.log\"" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "echo \"Running postinst at \$$(date)\" > \"\$$LOG\"" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "# Determine HOME directory" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "if [ -n \"\$$SUDO_USER\" ]; then HOME=\"/home/\$$SUDO_USER\"; else HOME=\"/home/\$$(whoami)\"; fi" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "echo \"HOME=\$$HOME\" >> \"\$$LOG\"" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "CONFIG_DIR=\"\$$HOME/.config/apxterm\"" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "if [ ! -d \"\$$CONFIG_DIR\" ]; then" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "    echo \"Creating \$$CONFIG_DIR\" >> \"\$$LOG\"" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "    mkdir -p \"\$$CONFIG_DIR\" || { echo \"Failed to create \$$CONFIG_DIR: \$$?\" >> \"\$$LOG\"; exit 1; }" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "else" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "    echo \"\$$CONFIG_DIR already exists\" >> \"\$$LOG\"" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "fi" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "SRC=\"/usr/share/apxterm/config.json\"" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "DEST=\"\$$CONFIG_DIR/config.json\"" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "if [ ! -f \"\$$DEST\" ]; then" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "    echo \"Copying \$$SRC to \$$DEST\" >> \"\$$LOG\"" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "    cp \"\$$SRC\" \"\$$DEST\" || { echo \"Failed to copy \$$SRC to \$$DEST: \$$?\" >> \"\$$LOG\"; exit 1; }" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "    chmod 644 \"\$$DEST\" || echo \"Failed to set permissions on \$$DEST\" >> \"\$$LOG\"" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "else" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "    echo \"\$$DEST already exists, skipping copy\" >> \"\$$LOG\"" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "fi" >> $(DEB_DIR)/DEBIAN/postinst
	@echo "echo \"Postinst completed successfully\" >> \"\$$LOG\"" >> $(DEB_DIR)/DEBIAN/postinst
	@chmod 755 $(DEB_DIR)/DEBIAN/postinst
	@chmod 755 $(DEB_DIR)/usr/bin/apxterm
	@chmod 644 $(DEB_DIR)/usr/share/apxterm/config.json
	dpkg-deb --build $(DEB_DIR) $(DEB_PACKAGE)

	rm -r debian

.PHONY: all run clean deb