cmake_minimum_required(VERSION 3.18)

project("playground")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PLAYGROUND_FILES CONFIGURE_DEPENDS
    ${PLAYGROUND_ROOT_DIR}/system/**.h
    ${PLAYGROUND_ROOT_DIR}/system/**.cpp
)

source_group(TREE ${PLAYGROUND_ROOT_DIR} FILES PLAYGROUND_FILES)

add_executable(${PROJECT_NAME}
    ${PLAYGROUND_FILES}
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${ENGINE_ROOT_DIR}
    PUBLIC ${PLAYGROUND_ROOT_DIR}
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE engine
    PRIVATE SDL3::SDL3-shared
)
target_compile_definitions(${PROJECT_NAME}
    PUBLIC $<$<CONFIG:Debug>:PLAYGROUND_DEBUG=1>
    PUBLIC $<$<CONFIG:Release>:PLAYGROUND_DEBUG=0>
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:SDL3::SDL3-shared>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_PDB_FILE:SDL3::SDL3-shared>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    VERBATIM
)

group_sources(${PROJECT_NAME} ${PLAYGROUND_ROOT_DIR})
