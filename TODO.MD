# TODO

This file outlines the remaining tasks to complete the Linux port of MilkDrop3.

## Current Blocker

The project is currently blocked from compiling due to a series of compilation and linker errors. The main issues are:

- **Missing `y.tab.h`:** The `nseel-lextab.c` file is missing the `y.tab.h` header, which is a generated file from the `yacc` or `bison` parser generator. This file needs to be generated as part of the build process.
- **Linker Errors:** The project is still experiencing linker errors related to multiple definitions and undefined references. This indicates that the `CMakeLists.txt` file still needs to be refined to correctly include all the necessary source files and to handle the dependencies between them.

## High Priority

- **Port Rendering from DirectX to OpenGL:** This is the largest remaining task. The core rendering logic in `vis_milk2/` is still heavily dependent on DirectX 9.
    - **`plugin.cpp`:** This file contains the main rendering loop and DirectX API calls. It needs to be refactored to use OpenGL.
    - **`support.cpp`:** The helper functions for 3D math need to be re-implemented using an OpenGL-friendly math library like GLM.
    - **Shaders:** The DirectX HLSL shaders need to be ported to GLSL.
    - **Vertex and Texture Management:** All code related to creating and managing vertex buffers, vertex declarations, and textures needs to be ported from DirectX to OpenGL (VBOs, VAOs, `glTexImage2D`, etc.).

- **Remove Windows API Dependencies:** Many files in `vis_milk2/` still use Windows API elements for UI and other functions. These need to be replaced with cross-platform alternatives (e.g., using GLFW for windowing and input) or removed.
    - **`pluginshell.h` / `pluginshell.cpp`:** Contains the main window procedure (`WindowProc`) and handles Windows messages (e.g., `WM_KEYDOWN`). This needs to be completely refactored for a GLFW-based event loop.
    - **`menu.h` / `menu.cpp`:** The menu system is based on the Windows API and needs to be replaced, possibly with a simple on-screen menu rendered with OpenGL.
    - **Various types:** `RECT`, `LRESULT`, `CALLBACK`, `HWND`, `WPARAM`, `LPARAM` need to be removed.

## Medium Priority

- **`libsamplerate` Dependency:** The new `EEL_VM` library has an optional dependency on `libsamplerate` which is currently missing. The build may fail on code paths that require it. We need to either provide this dependency or ensure the code paths are not used.

- **Cleanup Stubbed Headers:** The headers `support.h` and `texmgr.h` are currently stubbed out on Linux. As the OpenGL port progresses, these stubs should be replaced with proper OpenGL-based implementations.

- **Address Compiler Warnings:** The build still produces a number of warnings related to `const`-correctness and other issues in the `ns-eel2` library. These should be cleaned up.

## Low Priority

- **Review and Refine Build System:** The `CMakeLists.txt` file has been modified to get the project compiling, but it could be improved and organized for better cross-platform support.