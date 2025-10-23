cmake_minimum_required(VERSION 3.25)

# ============================================================================
# Destruction II - Modernized for Windows 10/11 x64
# Original: Johan Torp / Nocturnal Development (2001-2003)
# Modernization: 2025 - Full DirectX→SDL2 port
# ============================================================================

project(DestructionII
    VERSION 1.0.0
    DESCRIPTION "Tactical tank/artillery turn-based game"
    LANGUAGES CXX
)

# ============================================================================
# Build Configuration
# ============================================================================

# Force x64 architecture
set(CMAKE_GENERATOR_PLATFORM x64)

# C++20 standard (modern C++ features)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ============================================================================
# Compiler Settings
# ============================================================================

if(MSVC)
    # MSVC-specific flags for Windows 10/11
    add_compile_options(
        /W4                 # Warning level 4
        /WX-                # Warnings are not errors (for now)
        /MP                 # Multi-processor compilation
        /permissive-        # Conformance mode
        /Zc:__cplusplus     # Correct __cplusplus macro
        /utf-8              # UTF-8 source and execution charset
        /EHsc               # Exception handling
        /nologo             # Suppress startup banner
        /FI"${CMAKE_SOURCE_DIR}/Code/SDL2_DirectDrawCompat.h"  # Force include SDL2 compat layer
        /FI"${CMAKE_SOURCE_DIR}/Code/SDL2_DirectInputCompat.h"  # Force include SDL2 compat layer
        /FI"${CMAKE_SOURCE_DIR}/Code/SDL2_DirectSoundCompat.h"  # Force include SDL2 compat layer
    )

    # Release optimizations
    add_compile_options(
        $<$<CONFIG:Release>:/O2>     # Maximum speed
        $<$<CONFIG:Release>:/Ob2>    # Inline expansion
        $<$<CONFIG:Release>:/Oi>     # Intrinsic functions
        $<$<CONFIG:Release>:/Ot>     # Favor fast code
        $<$<CONFIG:Release>:/GL>     # Whole program optimization
    )

    # Linker flags
    add_link_options(
        $<$<CONFIG:Release>:/LTCG>   # Link-time code generation
        $<$<CONFIG:Release>:/OPT:REF> # Remove unreferenced functions
        $<$<CONFIG:Release>:/OPT:ICF> # Identical COMDAT folding
    )

    # Windows subsystem
    add_link_options(/SUBSYSTEM:WINDOWS)

    # Disable annoying MSVC warnings
    add_compile_definitions(
        _CRT_SECURE_NO_WARNINGS      # Allow strcpy, sprintf, etc.
        _CRT_NONSTDC_NO_DEPRECATE    # Allow POSIX names
        WIN32_LEAN_AND_MEAN          # Exclude rarely-used Windows headers
        NOMINMAX                     # Don't define min/max macros
    )
endif()

# ============================================================================
# Dependencies (SDL2 via vcpkg or find_package)
# ============================================================================

# Find SDL2 packages
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

# ============================================================================
# Source Files
# ============================================================================

# Collect all source files from Code/ directory
# Exclude Old Code/ directory
file(GLOB_RECURSE GAME_SOURCES
    "${CMAKE_SOURCE_DIR}/Code/*.cpp"
)

# Remove old code files
list(FILTER GAME_SOURCES EXCLUDE REGEX ".*Old Code.*")

# Exclude diutil.cpp (Force Feedback utilities - not needed for keyboard/mouse gameplay)
# This file requires extensive DirectInput FF structures that we don't need to implement
list(FILTER GAME_SOURCES EXCLUDE REGEX ".*diutil\\.cpp$")

# Collect all header files
file(GLOB_RECURSE GAME_HEADERS
    "${CMAKE_SOURCE_DIR}/Code/*.h"
)

# Remove old code headers
list(FILTER GAME_HEADERS EXCLUDE REGEX ".*Old Code.*")

# Print file count for verification
list(LENGTH GAME_SOURCES SOURCE_COUNT)
list(LENGTH GAME_HEADERS HEADER_COUNT)
message(STATUS "Found ${SOURCE_COUNT} source files")
message(STATUS "Found ${HEADER_COUNT} header files")

# ============================================================================
# Executable Target
# ============================================================================

add_executable(${PROJECT_NAME} WIN32
    ${GAME_SOURCES}
    ${GAME_HEADERS}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/Code
    ${CMAKE_SOURCE_DIR}/Code/include
)

# Link SDL2 libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2-static>
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
)

# Link Windows libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    winmm      # Windows Multimedia API
    comctl32   # Common Controls
)

# Preprocessor definitions
# __DIGITALRIVERRELEASE__ уже определён в Code/Definitions.h
# target_compile_definitions(${PROJECT_NAME} PRIVATE
#     # DirectX compatibility layer definitions can be added here if needed
# )

# ============================================================================
# Resource Files (Copy to build directory)
# ============================================================================

# Copy resource directories to build output
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Bitmaps
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Bitmaps
    COMMENT "Copying Bitmaps to build directory"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/RunDir/Sound
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Sound
    COMMENT "Copying Sound to build directory"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/RunDir/Data
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Data
    COMMENT "Copying Data to build directory"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Players
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Save
    COMMENT "Creating save directories"
)

# ============================================================================
# Installation
# ============================================================================

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/Bitmaps
    DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/RunDir/Sound
    DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/RunDir/Data
    DESTINATION bin
)

# ============================================================================
# Development Helper: Generate compile_commands.json for IDE support
# ============================================================================

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Status Messages
# ============================================================================

message(STATUS "==============================================")
message(STATUS "Destruction II - Build Configuration")
message(STATUS "==============================================")
message(STATUS "  CMake version:     ${CMAKE_VERSION}")
message(STATUS "  Generator:         ${CMAKE_GENERATOR}")
message(STATUS "  C++ Standard:      C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Architecture:      x64")
message(STATUS "  Build Type:        ${CMAKE_BUILD_TYPE}")
message(STATUS "  Compiler:          ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Output Directory:  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "==============================================")
