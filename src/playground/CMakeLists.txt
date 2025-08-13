cmake_minimum_required(VERSION 3.18)

project("playground")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PLAYGROUND_FILES CONFIGURE_DEPENDS
    ${PLAYGROUND_ROOT_DIR}/system/**.h
    ${PLAYGROUND_ROOT_DIR}/system/**.cpp
)

source_group(TREE ${PLAYGROUND_ROOT_DIR} FILES PLAYGROUND_FILES)

add_library(${PROJECT_NAME} STATIC
    ${PLAYGROUND_FILES}
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${ENGINE_ROOT_DIR}
    PUBLIC ${PLAYGROUND_ROOT_DIR}
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE engine
)
target_compile_definitions(${PROJECT_NAME}
    PUBLIC $<$<CONFIG:Debug>:PLAYGROUND_DEBUG=1>
    PUBLIC $<$<CONFIG:Release>:PLAYGROUND_DEBUG=0>
)

group_sources(${PROJECT_NAME} ${PLAYGROUND_ROOT_DIR})
