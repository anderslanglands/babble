cmake_minimum_required(VERSION 3.15)
project(inheritance VERSION 0.1 LANGUAGES C CXX)

# The bindfile contains all our binding definitions
set(bindfiles 
    bind.cpp
)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../../bbl/cmake)
include(babble-functions)

bbl_translate_binding(
    inheritance
    BINDFILES 
        ${bindfiles}
)

add_executable(test_inheritance test_inheritance.cpp)
target_link_libraries(test_inheritance PUBLIC inheritance-c)
target_include_directories(test_inheritance PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# # This is just here to trigger generation of compile commands for the bind file so we get LSP functionality in the bind file
add_library(inheritance-bind-dummy ${bindfiles})
target_link_libraries(inheritance-bind-dummy babble::bbl)


