find_package(Qt5Core)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)
find_package(Qt5Network)
find_package(Qt5Gui)
find_package(Qt5Charts)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

add_executable(templex_frontend
    src/main.cpp
    resources/res.qrc

    src/helpers/MenuRegistry.cpp

    src/ui/MainWindow.cpp
    src/ui/PageBase.cpp
    src/ui/BasicDelegate.cpp

    src/ui/common/class_template_selector/ClassTemplateSelector.cpp
    src/ui/common/class_template_selector/TemplateModel.cpp
    src/ui/common/class_template_selector/ClassNameFilterProxy.cpp

    src/ui/classes/instantiations/ClassInstantiationsPage.cpp
    src/ui/classes/instantiations/InstantiationsModel.cpp    
    src/ui/classes/instantiations/InstantiationSortingProxy.cpp

    src/ui/classes/statistics/ClassStatisticsPage.cpp

    src/ui/classes/stl_containers/STLContainersPage.cpp
    src/ui/classes/stl_containers/AggregationModel.cpp
    src/ui/classes/stl_containers/AggregationProxy.cpp
    src/ui/classes/stl_containers/InstantiationLocationModel.cpp

    src/ui/functions/instantiations/FunctionInstantiationsPage.cpp
    src/ui/functions/instantiations/FunctionNamesModel.cpp
    src/ui/functions/instantiations/FunctionOverloadsModel.cpp

    src/util/JSONImport.cpp)

set_property(TARGET templex_frontend PROPERTY CXX_STANDARD 17)
set_property(TARGET templex_frontend PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(templex_frontend
    templex_common
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    Qt5::Gui
    Qt5::Charts)

target_include_directories(templex_frontend
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/
    PRIVATE ${CMAKE_BINARY_DIR})
