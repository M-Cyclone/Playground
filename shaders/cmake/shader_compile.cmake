function(compile_shader SHADERS TARGET_NAME SHADER_INCLUDE_FOLDER GENERATED_DIR DXC_BIN)
    set(working_dir "${SHADER_ROOT_DIR}")

    set(ALL_GENERATED_DXIL_FILES "")
    set(ALL_GENERATED_CPP_FILES "")

    if(UNIX)
        execute_process(COMMAND chmod a+x ${DXC_BIN})
    endif()

    foreach(SHADER ${SHADERS})
        # Prepare a header name and a global variable for this shader
        get_filename_component(SHADER_NAME ${SHADER} NAME)
        get_filename_component(SHADER_EXT ${SHADER} EXT)

        string(REPLACE "." "_" HEADER_NAME ${SHADER_NAME})
        string(TOUPPER ${HEADER_NAME} GLOBAL_SHADER_VAR)

        set(DXIL_FILE "${SHADER_ROOT_DIR}/${GENERATED_DIR}/dxil/${SHADER_NAME}.dxil")
        set(CPP_FILE "${SHADER_ROOT_DIR}/${GENERATED_DIR}/cpp/${HEADER_NAME}.h")

        if(SHADER_EXT STREQUAL ".vert")
            add_custom_command(
                OUTPUT ${DXIL_FILE}
                COMMAND ${DXC_BIN} -T vs_6_0 -E main ${SHADER} -I ${SHADER_INCLUDE_FOLDER} -Fo ${DXIL_FILE}
                DEPENDS ${SHADER}
                WORKING_DIRECTORY "${working_dir}"
            )
        elseif(SHADER_EXT STREQUAL ".frag")
            add_custom_command(
                OUTPUT ${DXIL_FILE}
                COMMAND ${DXC_BIN} -T ps_6_0 -E main ${SHADER} -I ${SHADER_INCLUDE_FOLDER} -Fo ${DXIL_FILE}
                DEPENDS ${SHADER}
                WORKING_DIRECTORY "${working_dir}"
            )
        elseif(SHADER_EXT STREQUAL ".comp")
            add_custom_command(
                OUTPUT ${DXIL_FILE}
                COMMAND ${DXC_BIN} -T cs_6_0 -E main ${SHADER} -I ${SHADER_INCLUDE_FOLDER} -Fo ${DXIL_FILE}
                DEPENDS ${SHADER}
                WORKING_DIRECTORY "${working_dir}"
            )
        endif()

        list(APPEND ALL_GENERATED_DXIL_FILES ${DXIL_FILE})

        add_custom_command(
            OUTPUT ${CPP_FILE}
            COMMAND ${CMAKE_COMMAND} -DPATH=${DXIL_FILE} -DHEADER="${CPP_FILE}"
            -DGLOBAL="${GLOBAL_SHADER_VAR}" -P "${SHADER_ROOT_DIR}/cmake/generate_shader_cpp_file.cmake"
            DEPENDS ${DXIL_FILE}
            WORKING_DIRECTORY "${working_dir}"
        )

        list(APPEND ALL_GENERATED_CPP_FILES ${CPP_FILE})
    endforeach()

    add_custom_target(${TARGET_NAME}
        DEPENDS ${ALL_GENERATED_DXIL_FILES} ${ALL_GENERATED_CPP_FILES} SOURCES ${SHADERS}
    )
endfunction()
