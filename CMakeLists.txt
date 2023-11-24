cmake_minimum_required(VERSION  3.15)
project(babble VERSION 0.5.0 LANGUAGES C CXX)

find_package(Clang REQUIRED)
string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)"
       LLVM_VERSION_MATCH ${LLVM_VERSION})
set(LLVM_VERSION_MAJOR ${CMAKE_MATCH_1})
set(LLVM_VERSION_MINOR ${CMAKE_MATCH_2})
set(LLVM_VERSION_PATCH ${CMAKE_MATCH_3})

set(CMAKE_CXX_STANDARD 17)

include(GNUInstallDirs)

# Allow find_package to use <package>_ROOT variables when finding packages.
if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

# Make a usable debug target for MSVC
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
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(bbl)

# Define required locations in the build tree - these will be set automatically in babble-config
# during installation to the final locations
set(BABBLE_TRANSLATE $<TARGET_FILE:bbl-translate>)
set(BABBLE_RESOURCE_DIR "${CLANG_INSTALL_PREFIX}/lib/clang/${LLVM_VERSION_MAJOR}")

# more complete example bindings
add_subdirectory(test/smartptr)
add_subdirectory(test/stdfunction)

# plugins
add_subdirectory(plugins)