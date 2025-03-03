#!/bin/bash
set -e

# Variables
VERSION="1.0-1"  # Can be overridden or sourced later
DEB_DIR="build/debian"
TARGET="bin/apxterm"
DEB_PACKAGE="apxterm_${VERSION}_amd64.deb"

# Ensure binary exists
if [ ! -f "$TARGET" ]; then
    echo "Error: $TARGET not built. Run 'make' first."
    ls bin
    exit 1
fi

# Create Debian package structure
mkdir -p "$DEB_DIR/usr/bin"
mkdir -p "$DEB_DIR/usr/share/apxterm"
mkdir -p "$DEB_DIR/DEBIAN"

# Copy files
cp "$TARGET" "$DEB_DIR/usr/bin/"
cp "config.json" "$DEB_DIR/usr/share/apxterm/config.json"
cp "build/postinst.sh" "$DEB_DIR/DEBIAN/postinst"

# Create control file
cat << EOF > "$DEB_DIR/DEBIAN/control"
Package: apxterm
Version: $VERSION
Section: utils
Priority: optional
Architecture: amd64
Depends: libgtk-3-0 (>= 3.0), libvte-2.91-0 (>= 0.50), libcjson1
Maintainer: AP2015 <arian.press2015@gmail.com>
Description: A MobaXterm for Linux
 apxterm is a GTK+-based terminal emulator with support for multiple SSH sessions.
EOF

# Set permissions
chmod 755 "$DEB_DIR/usr/bin/apxterm"
chmod 644 "$DEB_DIR/usr/share/apxterm/config.json"
chmod 755 "$DEB_DIR/DEBIAN/postinst"

# Build the .deb package
dpkg-deb --build "$DEB_DIR" "$DEB_PACKAGE"

rm -r $DEB_DIR

echo "Created $DEB_PACKAGE"