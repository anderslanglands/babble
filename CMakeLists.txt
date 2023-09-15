cmake_minimum_required(VERSION  3.15)
project(babble VERSION 0.1 LANGUAGES C CXX)

include(GNUInstallDirs)

# Allow find_package to use <package>_ROOT variables when finding packages.
if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

set(BUILD_SHARED_LIBS OFF)

set(SPDLOG_INSTALL ON)
add_subdirectory(ext/spdlog)
add_subdirectory(ext/tiny-process-library)

add_subdirectory(bbl)

add_subdirectory(test/bbl-usd)