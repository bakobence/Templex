cmake_minimum_required(VERSION 3.15.2)
project(templex VERSION 0.1 LANGUAGES CXX)

option(WITH_FRONTEND "enable frontend" ON)
option(WITH_PARSER "enable parser" OFF)

if (NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (WITH_FRONTEND)
    add_subdirectory(frontend)
endif()

if (WITH_PARSER)
    add_subdirectory(parser)
endif()
