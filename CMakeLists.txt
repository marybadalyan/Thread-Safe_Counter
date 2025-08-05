cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Thread-Safe_Counter)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(main main.cpp) 


if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Define the compiler and linker flags for the "TSan" build type.
    # CMake automatically uses these variables when CMAKE_BUILD_TYPE is set to TSan.
    set(CMAKE_CXX_FLAGS_TSAN "-g -fsanitize=thread"
        CACHE STRING "Flags for ThreadSanitizer build type")
    set(CMAKE_EXE_LINKER_FLAGS_TSAN "-fsanitize=thread -pthread"
        CACHE STRING "Linker flags for ThreadSanitizer build type")

    # (Optional) Add TSan to the list of build types so it appears in the CMake GUI
    set_property(CACHE CMAKE_BUILD_TYPE APPEND PROPERTY STRINGS TSan)
endif()