set(FRONTEND_SOURCES
    src/main.cpp)

add_executable(templex_frontend ${FRONTEND_SOURCES})

set_property(TARGET templex_frontend PROPERTY CXX_STANDARD 17)
set_property(TARGET templex_frontend PROPERTY CXX_STANDARD_REQUIRED ON)

install()