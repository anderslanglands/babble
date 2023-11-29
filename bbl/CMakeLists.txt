find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
# Generate info for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Simplify installation messages
set(CMAKE_INSTALL_MESSAGE LAZY)

# bbl
add_library(bbl STATIC 
  src/cpp_context.cpp
  src/capi.cpp
  src/astutil.cpp
  src/plugin.cpp
  src/api/api_cpp_context.cpp
  src/api/api_capi.cpp
)
add_library(babble::bbl ALIAS bbl)

target_link_libraries(
  bbl
    PUBLIC
      clangAST
      clangTooling 
      clangBasic 
      clangASTMatchers 
      ${CMAKE_THREAD_LIBS_INIT}
)

target_include_directories(
  bbl
    PUBLIC
      "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
      "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/../ext/spdlog/include
)

target_include_directories(bbl PRIVATE ${LLVM_INCLUDE_DIRS})


add_library(bind INTERFACE)
add_library(babble::bind ALIAS bind)
target_include_directories(
  bind
    INTERFACE
      "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
      "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# bbl-translate
add_executable(bbl-translate src/bbl-translate.cpp)
target_link_libraries(bbl-translate 
  PUBLIC
    bbl
)
target_include_directories(
  bbl-translate
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/../ext/spdlog/include
)

# bbl-genbind
add_executable(bbl-genbind src/bbl-genbind.cpp)
target_include_directories(bbl-genbind 
  PRIVATE ${LLVM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/../ext/spdlog/include
)
target_link_libraries(bbl-genbind 
  PRIVATE
    bbl
)

if(UNIX)
  target_compile_options(bbl PUBLIC -Wno-deprecated-declarations)
  target_compile_options(bbl-genbind PUBLIC -Wno-deprecated-declarations)
else()
  target_compile_options(bbl PUBLIC /bigobj)
endif()

# LLVM is normally built without RTTI. Be consistent with that.
if(UNIX AND NOT LLVM_ENABLE_RTTI)
  target_compile_options(bbl PRIVATE -fno-rtti)
  target_compile_options(bbl-genbind PRIVATE -fno-rtti)
endif()

install(
  TARGETS 
    bbl bbl-translate bbl-genbind bind
  EXPORT 
    bbl-targets
  LIBRARY 
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE 
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME 
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES 
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

set(
  HEADERS 
    include/bbl-context.h 
    include/bbl-capi.h 
    include/bbl-detail.h
    include/babble
    include/babble-std
)

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  EXPORT 
    bbl-targets
  FILE 
    bbl-targets.cmake
  NAMESPACE 
    babble::
  DESTINATION 
    ${CMAKE_INSTALL_LIBDIR}/cmake/babble
)

include(CMakePackageConfigHelpers)
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
configure_package_config_file(
  cmake/babble-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/babble-config.cmake
  INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/babble
  PATH_VARS 
    INCLUDE_INSTALL_DIR
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/babble-config-version.cmake
  COMPATIBILITY 
    SameMinorVersion # change this to SameMajorVersion when we hit 1.0
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/babble-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/babble-config-version.cmake
    cmake/babble-functions.cmake
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/babble
)

# copy the clang resource dir to the installation so we can pick up the headers
# first need to extract the major version to get the right path from the 
# llvm installation
install(
  DIRECTORY
    ${CLANG_INSTALL_PREFIX}/lib/clang/${LLVM_VERSION_MAJOR}/include
    DESTINATION
      ${CMAKE_INSTALL_LIBDIR}/resource
)

# also copy it to the binary dir so we can pick it up in the test runner
file(
  COPY 
    ${CLANG_INSTALL_PREFIX}/lib/clang/${LLVM_VERSION_MAJOR}/include
  DESTINATION
    ${CMAKE_BINARY_DIR}
)