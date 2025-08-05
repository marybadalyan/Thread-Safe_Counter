cmake_minimum_required(VERSION 3.10)

project(ThreadSafeCounter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main main.cpp)

# Custom TSan build type setup
set(CMAKE_CXX_FLAGS_TSAN "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=thread -g -O1"
    CACHE STRING "Flags for ThreadSanitizer build type")
set(CMAKE_EXE_LINKER_FLAGS_TSAN "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=thread -pthread"
    CACHE STRING "Linker flags for ThreadSanitizer build type")

# (Optional) Add TSan to CMake GUI drop-down list
set_property(CACHE CMAKE_BUILD_TYPE APPEND PROPERTY STRINGS TSan)
