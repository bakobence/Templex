cmake_minimum_required(VERSION 3.15.2)
project(templex VERSION 0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GenerateExportHeader)

option(WITH_FRONTEND "enable frontend" OFF)
option(WITH_PARSER "enable parser" OFF)

if (NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(common)

if (WITH_FRONTEND)
    add_subdirectory(frontend)
endif()

if (WITH_PARSER)
    add_subdirectory(parser)
endif()
