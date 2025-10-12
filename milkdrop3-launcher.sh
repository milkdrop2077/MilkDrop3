#!/bin/bash
# MilkDrop3 Launcher for Linux
# This script launches MilkDrop3 under Wine with optimized settings

# Set Wine environment variables for better performance
export STAGING_SHARED_MEMORY=1
export WINEPREFIX="${WINEPREFIX:-$HOME/.wine}"

# MilkDrop3 installation directory
MILKDROP_DIR="$WINEPREFIX/drive_c/users/$USER/Desktop/MilkDrop 3.31"
MILKDROP_EXE="MilkDrop 3.exe"

# Check if MilkDrop3 is installed
if [ ! -d "$MILKDROP_DIR" ]; then
    echo "ERROR: MilkDrop3 not found at: $MILKDROP_DIR"
    echo "Please run the installer first. See LINUX_SETUP_GUIDE.md for instructions."
    exit 1
fi

if [ ! -f "$MILKDROP_DIR/$MILKDROP_EXE" ]; then
    echo "ERROR: MilkDrop 3.exe not found!"
    echo "Make sure you've replaced it with the Linux-compatible version."
    exit 1
fi

echo "Starting MilkDrop3..."
echo "Installation directory: $MILKDROP_DIR"
echo ""
echo "Keyboard shortcuts:"
echo "  F1  - Help menu"
echo "  F2  - Toggle FPS"
echo "  F7  - Toggle always on top"
echo "  L   - Load preset"
echo "  ESC - Exit"
echo ""

# Change to MilkDrop directory and launch
cd "$MILKDROP_DIR" || exit 1

# Suppress Wine debug output (comment out for troubleshooting)
wine "$MILKDROP_EXE" 2>/dev/null &

# Alternative: Show debug output (uncomment for troubleshooting)
# wine "$MILKDROP_EXE" &

echo "MilkDrop3 launched! (PID: $!)"
echo "Close this terminal or press Ctrl+C to keep it running in background."
