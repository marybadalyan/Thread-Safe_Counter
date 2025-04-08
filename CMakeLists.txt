cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Thread-Safe_Counter)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(Thread-Safe_Counter main.cpp)