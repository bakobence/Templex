set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(templex_parser
    src/main.cpp
    src/clang_implementation/RecursiveASTVisitor.cpp
    src/clang_implementation/RecursiveASTVisitor.h
    src/clang_implementation/ASTFrontendAction.cpp
    src/clang_implementation/ASTFrontendAction.h
    src/clang_implementation/ASTConsumer.cpp
    src/clang_implementation/ASTConsumer.h)

set_property(TARGET templex_parser PROPERTY CXX_STANDARD 17)
set_property(TARGET templex_parser PROPERTY CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION} in ${LLVM_INSTALL_PREFIX}")

find_package(Clang REQUIRED CONFIG HINTS "${LLVM_INSTALL_PREFIX}/lib/cmake/clang")
message(STATUS "Found Clang in ${CLANG_INSTALL_PREFIX}")

message(STATUS "LLVM built with RTTI: ${LLVM_ENABLE_RTTI}")

if (NOT LLVM_ENABLE_RTTI)
    message(SEND_ERROR "LLVM is compiled without RTTI")
endif ()

message("CLANG STATUS:
  Includes (clang)    ${CLANG_INCLUDE_DIRS}
  Includes (llvm)     ${LLVM_INCLUDE_DIRS}"
)

# PRIVATE since we don't want to expose our dependencies to our client application 
target_include_directories(templex_parser 
    SYSTEM
        PRIVATE ${LLVM_INCLUDE_DIRS}
        PRIVATE ${CLANG_INCLUDE_DIRS})

target_link_libraries(templex_parser PRIVATE LLVM clang-cpp)