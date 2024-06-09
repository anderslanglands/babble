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

    # Allow adding languages via cmake variable or env var
    list(APPEND arg_LANGUAGES ${BBL_LANGUAGES})
    list(APPEND arg_LANGUAGES $ENV{BBL_LANGUAGES})

    set(languages "")
    foreach(lang ${arg_LANGUAGES})
        list(APPEND languages "-l")
        list(APPEND languages "${lang}")
    endforeach()

    add_library(${TARGET_NAME} STATIC ${BBL_TRANSLATED_SOURCE})
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    # The bbl-translate binary will have the system include paths baked into it,
    # meaning that using the binary on a system with a different version of the
    # stdlib than the one on which it was built will fail to find the stdlib 
    # headers. 
    # In order to find them we need to parse the output of `c++ -v`, and add them
    # back with -isystem args
    if (UNIX OR APPLE)
        execute_process(COMMAND bash "-c" "c++ -xc++ /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,,p'" OUTPUT_VARIABLE gcc_default_includes)
        string(STRIP ${gcc_default_includes} gcc_default_includes)
        string(REPLACE "\n" ";" gcc_default_includes ${gcc_default_includes})
        foreach(inc ${gcc_default_includes})
            list(APPEND gcc_include_list "-isystem${inc}")
        endforeach()
    endif()

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
                ${gcc_include_list}
                -I${BABBLE_RESOURCE_DIR}/include
                "-I$<JOIN:$<TARGET_PROPERTY:babble::bind,INTERFACE_INCLUDE_DIRECTORIES>,;-I>" 
                "-I$<JOIN:$<TARGET_PROPERTY:${TARGET_NAME},INCLUDE_DIRECTORIES>,;-I>" 
                "$<JOIN:$<TARGET_PROPERTY:${TARGET_NAME},COMPILE_DEFINITIONS>,;>" 
                "$<JOIN:$<TARGET_PROPERTY:${TARGET_NAME},COMPILE_OPTIONS>,;>" 
                ${arg_COMPILE_ARGS}
                -- 
                ${PROJECT_NAME} 
                -o ${CMAKE_CURRENT_BINARY_DIR}
                ${languages}
        COMMAND_EXPAND_LISTS
    )

    # Ouptut the link libraries to a text file we can read downstream
    # First generate a stub main source file, then compile and link it with our
    # echo linker
    file(GENERATE OUTPUT ${PROJECT_NAME}-link-libraries.cpp CONTENT "int main() {return 0;}")
    add_executable(${PROJECT_NAME}-link-libraries ${PROJECT_NAME}-link-libraries.cpp)
    target_link_libraries(${PROJECT_NAME}-link-libraries ${TARGET_NAME})

    set_target_properties(
        ${PROJECT_NAME}-link-libraries
            PROPERTIES
                LINKER_LANGUAGE
                    ECHO
                SUFFIX
                    ".txt"
    )
endfunction()

function(BBL_GENERATE_BINDING PROJECT_NAME GENFILE NAMESPACE)
    # God, I hate CMake so much...
    set(TARGET_NAME "${PROJECT_NAME}-gen")
    set(OUTPUT_PROJECT_PATH ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
    set(OUTPUT_DUMMY ${OUTPUT_PROJECT_PATH}/bbl-dummy-cpp)

    set(list_args COMPILE_ARGS)
    cmake_parse_arguments(arg "${flags}" "${args}" "${list_args}" ${ARGN})


    add_library(${TARGET_NAME} SHARED ${GENFILE} ${OUTPUT_DUMMY})
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    set(genfile_path ${CMAKE_CURRENT_SOURCE_DIR}/${GENFILE})

    add_custom_command(
        OUTPUT ${OUTPUT_DUMMY}
        DEPENDS ${genfile_path}
        COMMAND 
            ${BABBLE_GENERATE}
                ${genfile_path} 
                -- 
                -std=c++17
                -fsyntax-only
                -fno-spell-checking
                -idirafter ${BABBLE_RESOURCE_DIR}/include
                "-I$<JOIN:$<TARGET_PROPERTY:babble::bind,INTERFACE_INCLUDE_DIRECTORIES>,;-I>" 
                "-I$<JOIN:$<TARGET_PROPERTY:${TARGET_NAME},INCLUDE_DIRECTORIES>,;-I>" 
                "$<JOIN:$<TARGET_PROPERTY:${TARGET_NAME},COMPILE_DEFINITIONS>,;>" 
                "$<JOIN:$<TARGET_PROPERTY:${TARGET_NAME},COMPILE_OPTIONS>,;>" 
                ${arg_COMPILE_ARGS}
                -- 
                ${PROJECT_NAME} 
                ${OUTPUT_PROJECT_PATH}
                ${NAMESPACE}
        COMMAND_EXPAND_LISTS
    )

endfunction()

