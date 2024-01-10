cmake_minimum_required(VERSION 3.19)
project(EZFrameless VERSION 1.0.0.0 LANGUAGES CXX)

option(EZFRAMELESS_BUILD_EXAMPLES "Build examples" OFF)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Add subdirectories
add_subdirectory(src)
if(EZFRAMELESS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
