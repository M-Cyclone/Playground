cmake_minimum_required(VERSION 3.18)

project("misaka-playground")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(SHADER_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(ENGINE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/engine")
set(PLAYGROUND_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/playground")

include(${PROJECT_ROOT_DIR}/cmake/group_utils.cmake)

# SDL
set(SDL_TEST_LIBRARY OFF)
add_subdirectory(${PROJECT_ROOT_DIR}/external/SDL)

if(TARGET SDL3)
    set_solution_folder(SDL3 "external/SDL3")
endif()

if(TARGET SDL3-shared)
    set_solution_folder(SDL3-shared "external/SDL3")
endif()

if(TARGET SDL_uclibc)
    set_solution_folder(SDL_uclibc "external/SDL3")
endif()

set(SHADER_COMPILE_TARGET shader-compile)
add_subdirectory(${SHADER_ROOT_DIR})

add_subdirectory(${ENGINE_ROOT_DIR})
add_subdirectory(${PLAYGROUND_ROOT_DIR})
