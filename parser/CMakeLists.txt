
set(PARSER_SOURCES
    src/main.cpp)

add_executable(templex_parser ${PARSER_SOURCES})

set_property(TARGET templex_parser PROPERTY CXX_STANDARD 17)
set_property(TARGET templex_parser PROPERTY CXX_STANDARD_REQUIRED ON)