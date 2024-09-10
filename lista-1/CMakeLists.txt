cmake_minimum_required(VERSION 3.30)

project(lista-1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory(../extern/glad glad)
add_subdirectory(../extern/glfw glfw)
add_subdirectory(src)