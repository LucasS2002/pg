cmake_minimum_required(VERSION 3.30)

project(lista-2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory(../extern/glad glad)
add_subdirectory(../extern/glfw glfw)
add_subdirectory(../extern/glm glm)
add_subdirectory(../lista-1/lib/glhelper glhelper)
add_subdirectory(src)