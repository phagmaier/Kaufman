cmake_minimum_required(VERSION 3.10)
project(TextEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Release if no build type is given (but allow user to override)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type (Debug, Release, etc.)")
endif()

# Debug/Release-specific compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -g")

# Add compiler warnings and colors for GCC/Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    -fdiagnostics-color=always
  )
endif()

# Include the source directory
include_directories(src)

# Source files
set(SOURCES
    src/main.cpp
    src/Gstring.cpp
    #src/Gbuff.cpp
    src/Editor.cpp
)

# Create the executable
add_executable(runme ${SOURCES})

# Link against ncurses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(runme ${CURSES_LIBRARIES})

