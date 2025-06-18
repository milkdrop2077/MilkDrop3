cmake_minimum_required(VERSION 3.10)

set(milkdrop_audio_SOURCES
    audiobuf.cpp
    # guid.cpp will be added conditionally
    log.cpp
    loopback-capture.cpp
    # prefs.cpp will be added conditionally
)

if(WIN32)
    list(APPEND milkdrop_audio_SOURCES guid.cpp)
    list(APPEND milkdrop_audio_SOURCES prefs.cpp)
endif()
# For Linux, GUID_NULL will be provided by linux_utility_stubs.cpp in vis_milk2
# and declared in the shim utility.h. If other GUIDs from guid.cpp were needed
# by cross-platform code, they would need similar handling.

add_library(milkdrop_audio ${milkdrop_audio_SOURCES})

target_include_directories(milkdrop_audio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add PulseAudio and PkgConfig for Linux builds
if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(PULSEAUDIO REQUIRED IMPORTED_TARGET libpulse-simple)

    if(PULSEAUDIO_FOUND)
        target_include_directories(milkdrop_audio PRIVATE ${PULSEAUDIO_INCLUDE_DIRS})
        target_link_libraries(milkdrop_audio PRIVATE PkgConfig::PULSEAUDIO)
        message(STATUS "PulseAudio found and configured for milkdrop_audio.")
    else()
        message(WARNING "PulseAudio (libpulse-simple) not found. Linux audio capture will not work.")
    endif()

    # Link pthread for Linux
    target_link_libraries(milkdrop_audio PRIVATE pthread)
endif()
