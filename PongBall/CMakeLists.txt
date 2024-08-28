cmake_minimum_required(VERSION 3.22)
project(run_game)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL3 package
find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

# Include SDL3 directories
include_directories(${SDL3_INCLUDE_DIRS})
include_directories(${SDL3_ttf_INCLUDE_DIRS})

# Add the source files
set(Sources main.cc game.cc)

# Add the executable
add_executable(run_game ${Sources})

# Link SDL3 library target_link_libraries(run_game ${SDL3_LIBRARIES}
# ${SDL3_ttf_LIBRARIES}) Link SDL3 and SDL3_ttf libraries
target_link_libraries(run_game SDL3::SDL3 SDL3_ttf::SDL3_ttf)
