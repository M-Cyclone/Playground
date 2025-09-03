set(TARGET_NAME engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ENGINE_FILES CONFIGURE_DEPENDS
    ${ENGINE_ROOT_DIR}/**.h
    ${ENGINE_ROOT_DIR}/**.cpp
)

source_group(TREE ${ENGINE_ROOT_DIR} FILES ENGINE_FILES)

add_library(${TARGET_NAME} STATIC
    ${ENGINE_FILES}
)
target_include_directories(${TARGET_NAME}
    PUBLIC ${ENGINE_ROOT_DIR}/include
)
target_compile_definitions(${TARGET_NAME}
    PUBLIC $<$<CONFIG:Debug>:ENGINE_DEBUG=1>
    PUBLIC $<$<CONFIG:Release>:ENGINE_DEBUG=0>
)
target_link_libraries(${TARGET_NAME}
    PUBLIC SDL3::SDL3-shared
)

group_sources(${TARGET_NAME} ${ENGINE_ROOT_DIR})
