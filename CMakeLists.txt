cmake_minimum_required(VERSION 3.22)
project(run_game)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL3 package
find_package(SDL3 REQUIRED)

# Include SDL3 directories
include_directories(${SDL3_INCLUDE_DIRS})

# Add the source files
set(Sources main.cc game.cc)

# Add the executable
add_executable(run_game ${Sources})

# Link SDL3 library
target_link_libraries(run_game ${SDL3_LIBRARIES})
