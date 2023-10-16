cmake_minimum_required(VERSION 3.15)
project(smartptr VERSION 0.1 LANGUAGES C CXX)

# The bindfile contains all our binding definitions
set(bindfiles 
    bind.cpp
)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../bbl/cmake)
include(babble-functions)

bbl_translate_binding(
    smartptr-c
    BINDFILES 
        ${bindfiles}
)

add_executable(test_smartptr test_smartptr.cpp)
target_link_libraries(test_smartptr PUBLIC smartptr-c)
target_include_directories(test_smartptr PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# # This is just here to trigger generation of compile commands for the bind file so we get LSP functionality in the bind file
add_library(smartptr-bind-dummy ${bindfiles})
target_link_libraries(smartptr-bind-dummy babble::bbl)

