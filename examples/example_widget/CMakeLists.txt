project(example_widget)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/qutils
    ${CMAKE_SOURCE_DIR}/qutils/src
)

set(EXAMPLE_WIDGET
    main.cpp
    widget.cpp
    widget.h
    rc.qrc
    IconFont.h
)

set(SRC_FILE_LIST
    ../../src/EZFrameless.h
    ../../src/EZFrameless.cpp
    ../../src/EZTitleBarBase.h
)

set(UTILS_FILE_LIST
    ../../qutils/src/FontManager.h
    ../../qutils/src/FontManager.cpp
)

add_executable(example_widget ${EXAMPLE_WIDGET} ${SRC_FILE_LIST} ${UTILS_FILE_LIST})
target_link_libraries(example_widget Qt${QT_VERSION_MAJOR}::Widgets dwmapi user32)
