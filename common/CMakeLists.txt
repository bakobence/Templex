
add_library(templex_common SHARED
    src/cache/TypeCache.cpp

    src/model/Instantiation.cpp
    src/model/Template.cpp
    src/model/TemplateParameter.cpp)

set_property(TARGET templex_common PROPERTY CXX_STANDARD 17)
set_property(TARGET templex_common PROPERTY CXX_STANDARD_REQUIRED ON)


target_include_directories(templex_common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR})

generate_export_header(templex_common)
