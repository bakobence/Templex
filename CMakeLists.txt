cmake_minimum_required(VERSION 3.15.2)
project(templex VERSION 0.1 LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(frontend)
add_subdirectory(parser)