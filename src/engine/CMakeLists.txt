set(TARGET_NAME engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

file(GLOB_RECURSE ENGINE_FILES CONFIGURE_DEPENDS
    ${ENGINE_ROOT_DIR}/core/**.h
    ${ENGINE_ROOT_DIR}/core/**.cpp
    
    ${ENGINE_ROOT_DIR}/math/**.h
    ${ENGINE_ROOT_DIR}/math/**.cpp
    
    ${ENGINE_ROOT_DIR}/sdl_wrapper/**.h
    ${ENGINE_ROOT_DIR}/sdl_wrapper/**.cpp
    
    ${ENGINE_ROOT_DIR}/system/**.h
    ${ENGINE_ROOT_DIR}/system/**.cpp
)

source_group(TREE ${ENGINE_ROOT_DIR} FILES ENGINE_FILES)

add_library(${TARGET_NAME} STATIC
    ${ENGINE_FILES}
)
target_include_directories(${TARGET_NAME}
    PUBLIC ${PROJECT_ROOT_DIR}/src
)
target_compile_definitions(${TARGET_NAME}
    PUBLIC $<$<CONFIG:Debug>:ENGINE_DEBUG=1>
    PUBLIC $<$<CONFIG:Release>:ENGINE_DEBUG=0>
)
target_link_libraries(${TARGET_NAME}
    PUBLIC SDL3::SDL3-shared
)

group_sources(${TARGET_NAME} ${ENGINE_ROOT_DIR})
