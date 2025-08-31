function(compile_shader SHADERS TARGET_NAME SHADER_INCLUDE_FOLDER GENERATED_DIR DXC_BIN)
    set(working_dir "${SHADER_ROOT_DIR}")

    set(ALL_GENERATED_BIN_FILES "")
    set(ALL_GENERATED_CPP_FILES "")

    if(UNIX)
        execute_process(COMMAND chmod a+x ${DXC_BIN})
    endif()

    set(SHADER_OPTIMIZATION_LEVEL 
        "$<$<CONFIG:Debug>:Od>$<$<CONFIG:Release>:O3>$<$<CONFIG:RelWithDebInfo>:O2>$<$<CONFIG:MinSizeRel>:Os>"
    )

    foreach(SHADER ${SHADERS})
        # Prepare a header name and a global variable for this shader
        get_filename_component(SHADER_NAME ${SHADER} NAME)
        get_filename_component(SHADER_EXT ${SHADER} EXT)

        string(REPLACE "." "_" HEADER_NAME ${SHADER_NAME})
        string(TOUPPER ${HEADER_NAME} GLOBAL_SHADER_VAR)

        set(BIN_FILE "${SHADER_ROOT_DIR}/${GENERATED_DIR}/bin/${SHADER_NAME}.bin")
        set(CPP_FILE "${SHADER_ROOT_DIR}/${GENERATED_DIR}/cpp/${HEADER_NAME}.h")

        set(SHADER_MODEL_TYPE "")

        if(SHADER_EXT STREQUAL ".vert")
            set(SHADER_MODEL_TYPE "vs_6_0")
        elseif(SHADER_EXT STREQUAL ".frag")
            set(SHADER_MODEL_TYPE "ps_6_0")
        elseif(SHADER_EXT STREQUAL ".comp")
            set(SHADER_MODEL_TYPE "cs_6_0")
        endif()

        if (NOT SHADER_MODEL_TYPE STREQUAL "")
            add_custom_command(
                OUTPUT ${BIN_FILE}
                COMMAND ${DXC_BIN} -T ${SHADER_MODEL_TYPE} -${SHADER_OPTIMIZATION_LEVEL} -E main ${SHADER} -I ${SHADER_INCLUDE_FOLDER} -Fo ${BIN_FILE}
                DEPENDS ${SHADER}
                WORKING_DIRECTORY "${working_dir}"
            )

            list(APPEND ALL_GENERATED_BIN_FILES ${BIN_FILE})

            add_custom_command(
                OUTPUT ${CPP_FILE}
                COMMAND ${CMAKE_COMMAND} -DPATH=${BIN_FILE} -DHEADER="${CPP_FILE}"
                -DGLOBAL="${GLOBAL_SHADER_VAR}" -P "${SHADER_ROOT_DIR}/cmake/generate_shader_cpp_file.cmake"
                DEPENDS ${BIN_FILE}
                WORKING_DIRECTORY "${working_dir}"
            )

            list(APPEND ALL_GENERATED_CPP_FILES ${CPP_FILE})
        endif()
    endforeach()

    add_custom_target(${TARGET_NAME}
        DEPENDS ${ALL_GENERATED_BIN_FILES} ${ALL_GENERATED_CPP_FILES} SOURCES ${SHADERS}
    )
endfunction()
