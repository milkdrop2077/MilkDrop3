cmake_minimum_required(VERSION 3.10)

set(milkdrop_vis_SOURCES
    Milkdrop2PcmVisualizer.cpp
    dxcontext.cpp
    fft.cpp
    menu.cpp
    milkdropfs.cpp
    plugin.cpp
    pluginshell.cpp
    state.cpp
    support.cpp
    texmgr.cpp
    textmgr.cpp
    # utility.cpp is added below conditionally
    wasabi.cpp
    glad/glad.c
)

if(WIN32)
    list(APPEND milkdrop_vis_SOURCES utility.cpp)
else()
    list(APPEND milkdrop_vis_SOURCES linux_utility_stubs.cpp)
endif()

add_library(milkdrop_vis ${milkdrop_vis_SOURCES})

# Common include directories for milkdrop_vis
target_include_directories(milkdrop_vis PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/glad" # For GLAD headers
    # Add parent directory to allow includes like "vis_milk2/some_header.h"
    # This is useful if other parts of the project include vis_milk2 headers that way.
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

# Platform-specific include directories for milkdrop_vis
if(NOT WIN32)
    # For Linux, add the specific path containing the shim utility.h
    # This allows #include "utility.h" to find the shim first.
    target_include_directories(milkdrop_vis PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/linux_includes/vis_milk2"
    )
endif()

# Add the current source directory as an include path.
# If the shim wasn't found in linux_includes/vis_milk2, this would find the original utility.h
# So the order is important.
# This allows includes like "utility.h" (if used directly) or "some_other_header.h".
target_include_directories(milkdrop_vis PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
# Include for project-level headers if vis_milk2 needs to include audio/ or ns-eel2/ directly
# For example, if plugin.cpp includes "../audio/common.h"
# This is already covered by CMAKE_SOURCE_DIR if includes are relative to project root.
# If includes are like <project_root>/code/audio/common.h, then ${CMAKE_SOURCE_DIR} is needed.
# The original target_include_directories(milkdrop_vis PUBLIC ${CMAKE_SOURCE_DIR}) might be too broad
# if it pulls in unrelated top-level project files.
# For now, assuming vis_milk2 includes are relative to its own dir or vis_milk2/..

# Link libraries based on platform
if(WIN32)
    # Windows specific libraries (e.g., D3D9, D3DX9)
    # These might be found via system paths or might need FindPackage(DirectX)
else()
    # Linux specific libraries: GLFW and OpenGL
    find_package(glfw3 3.3 REQUIRED)
    find_package(OpenGL REQUIRED)
    target_link_libraries(milkdrop_vis PRIVATE glfw ${OPENGL_LIBRARIES})
endif()

# Executable target (MilkDrop3)
add_executable(MilkDrop3 plugin.cpp)

target_link_libraries(MilkDrop3 PRIVATE milkdrop_vis milkdrop_audio milkdrop_nseel)

# Include directories for MilkDrop3 executable
target_include_directories(MilkDrop3 PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/glad"
    "${CMAKE_SOURCE_DIR}/audio"  # For #include "../audio/common.h" from plugin.cpp
    "${CMAKE_SOURCE_DIR}/ns-eel2"
)
if(NOT WIN32)
    target_include_directories(MilkDrop3 PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/linux_includes/vis_milk2"
    )
endif()
target_include_directories(MilkDrop3 PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
