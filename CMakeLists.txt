cmake_minimum_required(VERSION 3.10)
project(TextEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Release (optimized, disables asserts) if not specified
if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (Debug, Release, etc.)" FORCE)
endif()

# Set up warning flags for GCC/Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    -fdiagnostics-color=always
  )
endif()

# Set compiler flags for Release and Debug modes
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0")

# Always include the source directory
include_directories(src)

# Source files
set(SOURCES
    src/main.cpp
    src/Gstring.cpp
    src/Editor.cpp
)

# Find and link ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Main executable (always)
add_executable(runme ${SOURCES})
target_link_libraries(runme ${CURSES_LIBRARIES})

# In Debug mode, also build a "runme_debug" with debug flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_executable(runme_debug ${SOURCES})
  set_target_properties(runme_debug PROPERTIES COMPILE_FLAGS "-g -O0")
  target_link_libraries(runme_debug ${CURSES_LIBRARIES})
endif()
