cmake_minimum_required(VERSION 3.6)   # CMake version check
project(parallel-computing-project-P)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++11 standard


find_package (Threads)
set(SOURCE_FILES main.c structures.h
        world_structure.c world_structure.h
        read_value.c read_value.h
        )            # Add main.cpp file of project root directory as source file


add_executable(parallel_program ${SOURCE_FILES} )       # Add executable target with source files listed in SOURCE_FILES variable


target_link_libraries (parallel_program ${CMAKE_THREAD_LIBS_INIT})