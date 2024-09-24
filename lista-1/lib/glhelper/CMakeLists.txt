cmake_minimum_required(VERSION 3.30)

project(glhelper)

add_library(${PROJECT_NAME} STATIC include/glhelper/glhelper.hpp src/glhelper.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} glad glfw)