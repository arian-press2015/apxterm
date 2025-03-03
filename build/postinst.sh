#!/bin/sh
set -e

LOG="/tmp/apxterm_postinst.log"
echo "Running postinst at $(date)" > "$LOG"

# Determine HOME directory
if [ -n "$SUDO_USER" ]; then
    HOME="/home/$SUDO_USER"
else
    HOME="/home/$(whoami)"
fi
echo "HOME=$HOME" >> "$LOG"

CONFIG_DIR="$HOME/.config/apxterm"
if [ ! -d "$CONFIG_DIR" ]; then
    echo "Creating $CONFIG_DIR" >> "$LOG"
    mkdir -p "$CONFIG_DIR" || { echo "Failed to create $CONFIG_DIR: $?" >> "$LOG"; exit 1; }
else
    echo "$CONFIG_DIR already exists" >> "$LOG"
fi

SRC="/usr/share/apxterm/config.json"
DEST="$CONFIG_DIR/config.json"
if [ ! -f "$DEST" ]; then
    echo "Copying $SRC to $DEST" >> "$LOG"
    cp "$SRC" "$DEST" || { echo "Failed to copy $SRC to $DEST: $?" >> "$LOG"; exit 1; }
    chmod 644 "$DEST" || echo "Failed to set permissions on $DEST" >> "$LOG"
else
    echo "$DEST already exists, skipping copy" >> "$LOG"
fi

echo "Postinst completed successfully" >> "$LOG"