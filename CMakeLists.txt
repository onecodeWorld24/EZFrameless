cmake_minimum_required(VERSION 3.10)
project(EZFrameless VERSION 1.0.0.0 LANGUAGES CXX)

include_directories(
${CMAKE_SOURCE_DIR}/src
)

add_subdirectory(src)
add_subdirectory(examples/example_widget)
