
add_executable(templex_parser
    src/main.cpp
    
    src/clang_implementation/RecursiveASTVisitor.cpp
    src/clang_implementation/RecursiveASTVisitor.h
    src/clang_implementation/ASTFrontendAction.cpp
    src/clang_implementation/ASTFrontendAction.h
    src/clang_implementation/ASTConsumer.cpp
    src/clang_implementation/ASTConsumer.h
    src/util/JSONExport.cpp
    src/util/JSONExport.h)

set_property(TARGET templex_parser PROPERTY CXX_STANDARD 17)
set_property(TARGET templex_parser PROPERTY CXX_STANDARD_REQUIRED ON)

find_package(LLVM CONFIG)
message(STATUS "LLVM found. Headers: ${LLVM_INCLUDE_DIRS}")

find_package(Clang CONFIG HINTS "${LLVM_INSTALL_PREFIX}/lib/cmake/clang")
message(STATUS "Clang found Headers: ${CLANG_INCLUDE_DIRS}")

find_package(RapidJSON)

message(STATUS "RTTI: ${LLVM_ENABLE_RTTI}")

if (NOT LLVM_ENABLE_RTTI)
    message(SEND_ERROR "LLVM is compiled without RTTI")
endif ()

# PRIVATE since we don't want to expose our dependencies to our client application 
target_include_directories(templex_parser 
    SYSTEM
        PRIVATE ${LLVM_INCLUDE_DIRS}
        PRIVATE ${CLANG_INCLUDE_DIRS}
        PRIVATE ${RapidJSON_INCLUDE_DIR}
    )

target_include_directories(templex_parser
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/
    PRIVATE ${CMAKE_BINARY_DIR})

target_link_libraries(templex_parser PRIVATE LLVM clang-cpp templex_common)
