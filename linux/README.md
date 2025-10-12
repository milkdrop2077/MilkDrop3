# MilkDrop3 on Linux with Wine - Setup Guide

This guide will help you run MilkDrop3 on Linux using Wine.

## Prerequisites

- Linux distribution (tested on Manjaro with Wine 10.13 Staging)
- Wine (10.13 or later recommended, Wine Staging preferred)
- winetricks
- Basic terminal knowledge

## System Requirements

- **Wine version**: 10.13 or later (Wine Staging recommended)
- **Architecture**: 64-bit system with wow64 support
- **Graphics**: OpenGL-capable GPU with decent 3D support

## Installation Steps

### 1. Install Required Packages

#### On Arch/Manjaro:
```bash
sudo pacman -S wine winetricks
```

#### On Ubuntu/Debian:
```bash
sudo apt install wine winetricks
```

#### On Fedora:
```bash
sudo dnf install wine winetricks
```

### 2. Download MilkDrop3

Download the official MilkDrop3 installer:
```bash
wget -O MilkDrop3.exe "https://github.com/milkdrop2077/MilkDrop3/releases/download/MilkDrop3/MilkDrop3.exe"
```

### 3. Set Up Wine Environment

Install necessary Windows dependencies:
```bash
winetricks -q d3dx9 vcrun2019
```

This will install:
- DirectX 9 libraries (d3dx9)
- Visual C++ 2019 Redistributables

**Note**: This process may take 5-10 minutes depending on your internet connection.

### 4. Install MilkDrop3

Run the installer with Wine:
```bash
wine MilkDrop3.exe
```

Follow the on-screen installation prompts. The default installation location will be:
```
~/.wine/drive_c/users/YOUR_USERNAME/Desktop/MilkDrop 3.31/
```

### 5. Replace with Linux-Compatible Executable

MilkDrop3 requires a special Linux-compatible build. Replace the standard executable:

```bash
# Download the Linux-compatible version
wget -O "MilkDrop 3 linux.exe" "https://raw.githubusercontent.com/milkdrop2077/MilkDrop3/main/linux/MilkDrop%203%20linux.exe"

# Replace the executable (adjust path if needed)
cd ~/.wine/drive_c/users/$USER/Desktop/"MilkDrop 3.31"/
rm "MilkDrop 3.exe"
mv /path/to/downloaded/"MilkDrop 3 linux.exe" "MilkDrop 3.exe"
```

### 6. Run MilkDrop3

Navigate to the installation directory and run:
```bash
cd ~/.wine/drive_c/users/$USER/Desktop/"MilkDrop 3.31"/
wine "MilkDrop 3.exe"
```

## Using the Launcher Script

A convenient launcher script is included in the repository. Use it to easily start MilkDrop3:

```bash
# Download the launcher script from the repository
wget -O milkdrop3-launcher.sh "https://raw.githubusercontent.com/milkdrop2077/MilkDrop3/main/milkdrop3-launcher.sh"

# Make it executable
chmod +x milkdrop3-launcher.sh

# Run MilkDrop3
./milkdrop3-launcher.sh
```

The launcher script automatically:
- Sets optimal Wine environment variables for performance
- Verifies installation paths
- Displays helpful keyboard shortcuts
- Suppresses unnecessary debug output

## Known Issues and Workarounds

### Text Rendering Issues
As mentioned in the original README, menu text (F1 help, L/M menus) may appear glitchy. This is a known issue with the Wine rendering layer.

**Workaround**: The visualization itself works perfectly; you can navigate menus by memorizing keyboard shortcuts.

### D3D Texture Warnings
You may see numerous warnings like:
```
err:d3d:wined3d_texture_invalidate_location Sub-resource X does not have any up to date location
```

These are harmless Wine warnings related to texture management and don't affect functionality.

### GLSL Shader Warnings
Warnings about uninitialized shader variables are cosmetic and don't impact the visualizations.

### Performance Issues
If you experience performance issues:

1. **Use Wine Staging**: It has better DirectX support
2. **Enable CSMT**: Run with `STAGING_SHARED_MEMORY=1`
3. **Try different Wine versions**: Sometimes newer/older versions work better
4. **Check your GPU drivers**: Ensure you have the latest drivers installed

## Audio Configuration

MilkDrop3 needs an audio source to visualize. Options include:

1. **System Audio**:
   - Use PulseAudio/PipeWire loopback to route system audio to MilkDrop3
   - Install `pavucontrol` to manage audio routing

2. **Music Players**:
   - MilkDrop3 can capture audio from Spotify, YouTube, VLC, etc.
   - Make sure audio is playing before starting visualizations

3. **Configure in Wine**:
   ```bash
   winecfg
   ```
   Go to the Audio tab and ensure PulseAudio is selected.

## Keyboard Shortcuts (Essential for Linux)

Since menu text may be glitchy, here are the essential shortcuts:

- **F1**: Help menu (toggle)
- **F2**: Toggle FPS (60/90/120 fps)
- **F3**: Change auto-transition time
- **F7**: Toggle always on top / borderless mode
- **F8**: Beat detection mode
- **F9**: Double-preset mode
- **L**: Load preset
- **M**: Mash-up presets
- **Spacebar**: Next transition
- **A**: Random preset
- **Shift+A**: Previous preset
- **C**: Randomize colors
- **Escape**: Exit

## Troubleshooting

### Application Won't Start
```bash
# Check Wine is working
wine --version

# Reinstall dependencies
winetricks -q --force d3dx9 vcrun2019

# Try running with more verbose output
WINEDEBUG=+d3d wine "MilkDrop 3.exe"
```

### Crashes on Startup
- Ensure you're using the Linux-compatible executable
- Check that DirectX 9 libraries are installed
- Try running with a 32-bit Wine prefix:
  ```bash
  WINEARCH=win32 WINEPREFIX=~/.wine32 winetricks d3dx9 vcrun2019
  ```

### No Visualizations Appearing
- Ensure audio is playing
- Check audio input in Windows sound settings (via `winecfg`)
- Try running a music player first, then launch MilkDrop3

## Performance Optimization

### Environment Variables
```bash
# Enable Wine Staging shared memory (better performance)
export STAGING_SHARED_MEMORY=1

# Force OpenGL version
export MESA_GL_VERSION_OVERRIDE=4.5

# Run MilkDrop3
wine "MilkDrop 3.exe"
```

### Graphics Settings
In MilkDrop3:
- Lower resolution if needed (in settings.ini)
- Reduce FPS with F2 if experiencing lag
- Disable complex effects if GPU struggles

## Advanced: Desktop Entry (Optional)

Create a desktop launcher at `~/.local/share/applications/milkdrop3.desktop`:

```desktop
[Desktop Entry]
Name=MilkDrop3
Comment=Music Visualization
Exec=bash -c 'cd ~/.wine/drive_c/users/$USER/Desktop/"MilkDrop 3.31/" && wine "MilkDrop 3.exe"'
Icon=applications-multimedia
Terminal=false
Type=Application
Categories=AudioVideo;Audio;
```

## Additional Resources

- **Official Repository**: https://github.com/milkdrop2077/MilkDrop3
- **Preset Collections**: Located in `Milkdrop3/presets/`
- **Creating Presets**: Press F9 then Spacebar for double-presets
- **WineHQ**: https://www.winehq.org/ for Wine documentation

## Credits

- **MilkDrop3**: Created by MilkDrop2077
- **Original MilkDrop**: Ryan Geiss
- **BeatDrop**: Maxim Volskiy

## Support the Project

If you enjoy MilkDrop3, consider supporting the developer:
- Patreon: https://www.patreon.com/MilkDrop3
- Buy Me a Coffee: https://www.buymeacoffee.com/MilkDrop2077/

---

## Quick Start Summary

```bash
# 1. Install Wine and winetricks
sudo pacman -S wine winetricks  # Adjust command for your distro

# 2. Install Windows dependencies (takes 5-10 minutes)
winetricks -q d3dx9 vcrun2019

# 3. Download and install MilkDrop3
wget -O MilkDrop3.exe "https://github.com/milkdrop2077/MilkDrop3/releases/download/MilkDrop3/MilkDrop3.exe"
wine MilkDrop3.exe

# 4. Replace with Linux-compatible executable
cd ~/.wine/drive_c/users/$USER/Desktop/"MilkDrop 3.31"/
wget -O "MilkDrop 3.exe" "https://raw.githubusercontent.com/milkdrop2077/MilkDrop3/main/linux/MilkDrop%203%20linux.exe"

# 5. Download and run the launcher script
cd ~
wget -O milkdrop3-launcher.sh "https://raw.githubusercontent.com/milkdrop2077/MilkDrop3/main/linux/milkdrop3-launcher.sh"
chmod +x milkdrop3-launcher.sh
./milkdrop3-launcher.sh
```

**Enjoy the visualizations!** 🎵🎨

