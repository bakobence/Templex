find_package(Qt5Core)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets)
find_package(Qt5Network)
find_package(Qt5Gui)

add_executable(templex_frontend
    src/main.cpp

    resources/res.qrc

    src/MainWindow.cpp
    src/MainWindow.ui
    src/MainWindow.h

    src/helpers/GlobalDefines.h
    src/helpers/MenuRegistry.cpp
    src/helpers/MenuRegistry.h

    src/ui/classes/instantiations/ClassInstantiationsPage.cpp
    src/ui/classes/instantiations/ClassInstantiationsPage.h)

set_property(TARGET templex_frontend PROPERTY CXX_STANDARD 17)
set_property(TARGET templex_frontend PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(templex_frontend
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    Qt5::Gui)

target_include_directories(templex_frontend
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/
    PRIVATE ${CMAKE_BINARY_DIR})
