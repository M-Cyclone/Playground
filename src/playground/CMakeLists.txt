cmake_minimum_required(VERSION 3.18)

project("playground")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PLAYGROUND_FILES CONFIGURE_DEPENDS
    ${PLAYGROUND_ROOT_DIR}/core/**.h
    ${PLAYGROUND_ROOT_DIR}/core/**.cpp

    ${PLAYGROUND_ROOT_DIR}/system/**.h
    ${PLAYGROUND_ROOT_DIR}/system/**.cpp

    ${PLAYGROUND_ROOT_DIR}/utils/**.h
    ${PLAYGROUND_ROOT_DIR}/utils/**.cpp
)

source_group(TREE ${PLAYGROUND_ROOT_DIR} FILES PLAYGROUND_FILES)

add_executable(${PROJECT_NAME}
    ${PLAYGROUND_FILES}
)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_ROOT_DIR}/src
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
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_ROOT_DIR}/shaders/bin" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
    VERBATIM
)

group_sources(${PROJECT_NAME} ${PROJECT_ROOT_DIR})

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_ROOT_DIR}/")
