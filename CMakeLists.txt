cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Parser VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add the executable
add_executable(Parser parser.cpp)