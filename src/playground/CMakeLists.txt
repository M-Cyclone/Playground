# PackageProject.cmake will be used to make our target installable
set(TARGET_NAME playground)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PLAYGROUND_FILES CONFIGURE_DEPENDS
    ${PLAYGROUND_ROOT_DIR}/core/**.h
    ${PLAYGROUND_ROOT_DIR}/core/**.cpp

    ${PLAYGROUND_ROOT_DIR}/system/**.h
    ${PLAYGROUND_ROOT_DIR}/system/**.cpp

    ${PLAYGROUND_ROOT_DIR}/utils/**.h
    ${PLAYGROUND_ROOT_DIR}/utils/**.cpp

    ${PROJECT_ROOT_DIR}/shaders/include/**.h
    ${PROJECT_ROOT_DIR}/shaders/generated/cpp/**.h
)

source_group(TREE ${PLAYGROUND_ROOT_DIR} FILES PLAYGROUND_FILES)

add_executable(${TARGET_NAME}
    ${PLAYGROUND_FILES}
)
target_include_directories(${TARGET_NAME}
    PUBLIC ${PROJECT_ROOT_DIR}/src
    PUBLIC ${PROJECT_ROOT_DIR}/shaders/generated/cpp
)
target_link_libraries(${TARGET_NAME}
    PRIVATE engine
    PRIVATE SDL3::SDL3-shared
)
target_compile_definitions(${TARGET_NAME}
    PUBLIC $<$<CONFIG:Debug>:PLAYGROUND_DEBUG=1>
    PUBLIC $<$<CONFIG:Release>:PLAYGROUND_DEBUG=0>
)

add_dependencies(${TARGET_NAME} ${SHADER_COMPILE_TARGET})

add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:SDL3::SDL3-shared>" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    COMMAND $<$<NOT:$<CONFIG:Release>>:${CMAKE_COMMAND}> 
            $<$<NOT:$<CONFIG:Release>>:-E> 
            $<$<NOT:$<CONFIG:Release>>:copy> "$<TARGET_PDB_FILE:SDL3::SDL3-shared>" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    VERBATIM
)

group_sources(${TARGET_NAME} ${PROJECT_ROOT_DIR})
