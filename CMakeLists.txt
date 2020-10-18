cmake_minimum_required(VERSION 3.15.2)
project(templex VERSION 1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_subdirectory(frontend)
add_subdirectory(parser)

if (NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Debug)
endif()

# https://cmake.org/cmake/help/latest/variable/CMAKE_INCLUDE_CURRENT_DIR.html
#set(CMAKE_INCLUDE_CURRENT_DIR ON)



