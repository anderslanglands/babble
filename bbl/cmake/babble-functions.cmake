function(BBL_TRANSLATE_BINDING PROJECT_NAME)
    # God, I hate CMake so much...
    set(bindfiles)
    set(list_args BINDFILES COMPILE_ARGS LANGUAGES)

    cmake_parse_arguments(arg "${flags}" "${args}" "${list_args}" ${ARGN})

    if(NOT arg_BINDFILES)
        message(FATAL_ERROR "[translate_binding]: BINDFILES is a required argument")
    endif()

    if(BINDFILES IN_LIST arg_KEYWORDS_MISSING_VALUES)
        message(FATAL_ERROR "[translate_binding]: BINDFILES required at least one value")
    endif()

    set(TARGET_NAME "${PROJECT_NAME}-c")

    set(BBL_TRANSLATED_SOURCE "${TARGET_NAME}.cpp")
    set(BBL_TRANSLATED_HEADER "${TARGET_NAME}.h")

    set(bindfile_abs_args "")
    foreach(bindfile ${arg_BINDFILES})
        get_filename_component(_abs ${bindfile} REALPATH BASEDIR ${CMAKE_CURRENT_BINARY_DIR})
        list(APPEND bindfile_abs_args ${_abs})
    endforeach()

    add_library(${TARGET_NAME} STATIC ${BBL_TRANSLATED_SOURCE})
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    add_custom_command(
        OUTPUT ${BBL_TRANSLATED_SOURCE}
        DEPENDS ${arg_BINDFILES}
        COMMAND 
            ${BABBLE_TRANSLATE}
                ${bindfile_abs_args} 
                -- 
                -std=c++17
                -fsyntax-only
                -fno-spell-checking
                 -I${BABBLE_RESOURCE_DIR}/include
                "-I$<JOIN:$<TARGET_PROPERTY:babble::bind,INTERFACE_INCLUDE_DIRECTORIES>,;-I>" 
                "-I$<JOIN:$<TARGET_PROPERTY:${TARGET_NAME},INCLUDE_DIRECTORIES>,;-I>" 
                ${arg_COMPILE_ARGS}
                -- 
                ${PROJECT_NAME} 
                -o ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND_EXPAND_LISTS
    )

endfunction()

