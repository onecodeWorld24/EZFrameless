project(EZFrameless)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCE_FILES
    EZFrameless.h
    EZFrameless.cpp
    EZTitlebar.h
    EZTitlebar.cpp
)

add_library(EZFramless ${SOURCE_FILES})
target_link_libraries(EZFramless Qt${QT_VERSION_MAJOR}::Widgets)
