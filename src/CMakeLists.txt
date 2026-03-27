cmake_minimum_required(VERSION 3.16)
project(Water_Animation)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)

find_package(OpenGL REQUIRED)

add_executable(Water_Animation main.cpp OpenGLRenderer.cpp ./vendor/glad/src/glad.c Water_Animation.cpp)
target_include_directories(Water_Animation PUBLIC ./vendor/glad/include ./vendor/glm/glm)
target_link_libraries(Water_Animation PRIVATE SDL3::SDL3)