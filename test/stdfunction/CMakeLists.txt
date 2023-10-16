cmake_minimum_required(VERSION 3.15)
project(stdfunction VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../bbl/cmake)
include(babble-functions)

# The bindfile contains all our binding definitions
set(bindfiles 
    bind.cpp
)

bbl_translate_binding(
    stdfunction-c
    BINDFILES 
        ${bindfiles}
)

add_executable(test_stdfunction test_stdfunction.cpp)
target_link_libraries(test_stdfunction PUBLIC stdfunction-c)
target_include_directories(test_stdfunction PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# # This is just here to trigger generation of compile commands for the bind file so we get LSP functionality in the bind file
add_library(stdfunction-bind-dummy ${bindfiles})
target_link_libraries(stdfunction-bind-dummy babble::bbl)


