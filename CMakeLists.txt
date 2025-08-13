cmake_minimum_required(VERSION 3.18)

project("misaka-playground")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(ENGINE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/engine")
set(PLAYGROUND_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/playground")

include(${PROJECT_ROOT_DIR}/cmake/group_sources.cmake)

add_subdirectory(${ENGINE_ROOT_DIR})
add_subdirectory(${PLAYGROUND_ROOT_DIR})
