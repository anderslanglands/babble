cmake_minimum_required(VERSION  3.15)
project(babble VERSION 0.1 LANGUAGES C CXX)

option(BUILD_USD_EXAMPLE "Builds an example OpenUSD binding")

include(GNUInstallDirs)

# Allow find_package to use <package>_ROOT variables when finding packages.
if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

# Screw you, visual studio
if (MSVC)
    add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_SILENCE_ALL_CXX20_DEPRECATION_WARNINGS)
    # Disable optimizations in relwithdebinfo build on Windows so we can use it for debugging
    string(REPLACE "/O2" "/Od" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    string(REPLACE "/Ob1" "/Ob0" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    string(CONCAT CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" " /Zi")
endif()

set(BUILD_SHARED_LIBS OFF)

set(SPDLOG_INSTALL ON)
add_subdirectory(ext/spdlog)
add_subdirectory(ext/tiny-process-library)

add_subdirectory(bbl)

if(${BUILD_USD_EXAMPLE})
    find_package(pxr)

    if (${pxr_FOUND})
        add_subdirectory(test/bbl-usd)
    else()
        message(SEND_ERROR, "Option BUILD_USD_EXAMPLE is enabled, but package pxr was not found. Skipping USD example.")
    endif()
endif()