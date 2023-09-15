cmake_minimum_required(VERSION 3.15)
project(bbl-usd VERSION 0.1)

find_package(pxr REQUIRED)

add_library(bind-dummy bind.cpp)
target_link_libraries(bind-dummy usd babble::bbl)

set(SOURCE usd-c.cpp)
set(HEADER usd-c.h)

add_library(usd-c ${SOURCE})
target_link_libraries(usd-c PUBLIC usd)
add_compile_definitions(usd-c PRIVATE NOMINMAX BOOST_ALL_NO_LIB __TBB_show_deprecation_message_task_H)

get_filename_component(bind_abs bind.cpp REALPATH BASEDIR ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(
    OUTPUT ${SOURCE}
    DEPENDS ${bind_abs}
    COMMAND 
        bbl-translate 
            ${bind_abs} 
            -- 
            -std=c++17
            "-I$<JOIN:$<TARGET_PROPERTY:babble::bbl,INCLUDE_DIRECTORIES>,;-I>" 
            "-I$<JOIN:$<TARGET_PROPERTY:usd-c,INCLUDE_DIRECTORIES>,;-I>" 
            -Wno-deprecated-builtins -DNOMINMAX -D_MT -DBOOST_ALL_NO_LIB -D__TBB_show_deprecation_message_task_H 
            -- 
            ${SOURCE} ${HEADER}
    COMMAND_EXPAND_LISTS
)

add_executable(usd-c-test01 usd-c-test01.cpp)
target_link_libraries(usd-c-test01 PUBLIC usd-c)
target_include_directories(usd-c-test01 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})