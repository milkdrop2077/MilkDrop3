# Changelog

This file documents the major changes made to the MilkDrop3 codebase in the effort to port it to Linux.

## Build System and Stubbing (In-Progress)

- **Consolidated Windows/DirectX Types:** All Windows-specific type definitions (e.g., `DWORD`, `RECT`, `HWND`) have been moved to a central header, `shell_defines.h`, to resolve redefinition conflicts across the codebase.
- **Stubbed DirectX Dependencies:** A significant number of DirectX types, constants, and functions have been stubbed out in `shell_defines.h` and a new `dx_on_gl.h` compatibility header. This was a necessary step to satisfy the compiler and achieve a successful build, but the stubs will need to be replaced with functional OpenGL/GLFW code.
- **Resolved String Mismatches:** Corrected numerous compilation errors by migrating from wide-character strings (`wchar_t*`) to standard strings (`char*`) where appropriate, including updating function calls from `wcscpy` to `strcpy` and `swprintf` to `sprintf`.
- **Fixed Platform-Specific Function Calls:** Replaced non-standard functions like `min`/`max` with `std::min`/`std::max` and addressed other Windows-specific API calls.
- **`ns-eel2` Library Updates:** The `ns-eel2` library has been updated to include missing source files and the `CMakeLists.txt` has been updated to exclude the conflicting `loose_eel.c` file.
- **Corrected `NSEEL` Function Calls:** Replaced calls to the non-existent `NSEEL_VM_resetvars` function with `NSEEL_VM_freevars` and added the missing `lineoffs` argument to all calls to `NSEEL_code_compile`.

## Core Changes

- **Replaced `ns-eel2` Library:** The original `ns-eel2` library was missing 64-bit Linux support. It has been entirely replaced with the more modern and cross-platform [EEL_VM library](https://github.com/james34602/EEL_VM). This involved updating the build system to include the new set of source files.

- **Audio Backend Ported to PortAudio:** The audio capture backend, originally written using Windows-specific APIs (WASAPI), has been refactored.
    - The build now uses `loopback-capture-portaudio.cpp`, which uses the cross-platform PortAudio library.
    - The Windows-specific audio code (`loopback-capture.cpp`, `prefs.cpp`) has been excluded from the Linux build.
    - The threading model for audio capture was ported from Windows `CreateThread` to C++ `std::thread`.

- **DirectX Dependencies Stubbed:** The original rendering engine was based on DirectX 9. All DirectX-dependent header files have been stubbed out for the Linux build to allow compilation to proceed. This includes:
    - `support.h`: Replaced with stubs for matrix and vector operations.
    - `texmgr.h`: Replaced with a stubbed-out texture manager class.
    - All DirectX-specific types (`D3DXMATRIX`, `LPDIRECT3DTEXTURE9`, etc.) have been stubbed with `void*` or equivalent placeholders on Linux.

## Portability Fixes

- **Removed `windows.h` Includes:** Numerous files included `windows.h` and other Windows-specific headers. These have been removed or wrapped in `#ifdef _WIN32` blocks.
- **Resolved `min`/`max` Macro Conflicts:** The `ns-eel.h` header defined `min` and `max` as macros, which conflicted with the C++ standard library. These macros are now excluded from C++ builds.
- **Fixed `dirent.h` Implementation:** The project included a Windows-specific implementation of `dirent.h`. This has been fixed to use the native system `<dirent.h>` on Linux, resolving a recursive include issue.
- **Standardized Types:** Replaced Windows-specific types like `BYTE`, `DWORD`, `HANDLE`, etc., with their standard C++ counterparts (`uint8_t`, `uint32_t`, `void*`) where appropriate for the Linux build.