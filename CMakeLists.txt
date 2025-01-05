cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MisraPingPong)

# Add the executable
add_executable(MisraPingPong main.cpp sender.cpp receiver.cpp)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
