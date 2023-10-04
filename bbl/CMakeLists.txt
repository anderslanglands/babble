cmake_minimum_required(VERSION  3.15)
project(bbl VERSION 0.1 LANGUAGES C CXX)

find_package(Clang 16.0 REQUIRED)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
# Generate info for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Simplify installation messages
set(CMAKE_INSTALL_MESSAGE LAZY)

add_library(bbl STATIC 
  src/bbl.cpp
  src/bbl-c.cpp
  src/astutil.cpp
  src/matchers.cpp
  src/cpp_api.cpp
  src/c_api.cpp
)

target_link_libraries(
  bbl
    PUBLIC
      clangAST
      clangTooling 
      clangBasic 
      clangASTMatchers 
      spdlog::spdlog 
)

add_library(babble::bbl ALIAS bbl)

target_include_directories(
  bbl
    PUBLIC
      "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
      "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_include_directories(bbl PRIVATE ${LLVM_INCLUDE_DIRS})

add_executable(bbl-translate src/bbl-translate.cpp)

target_link_libraries(bbl-translate 
  bbl
  spdlog::spdlog 
)

# LLVM is normally built without RTTI. Be consistent with that.
if(UNIX AND NOT LLVM_ENABLE_RTTI)
  target_compile_options(bbl PUBLIC -fno-rtti)
endif()

install(
  TARGETS 
    bbl bbl-translate
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
    include/bbl.h 
    include/bbl-c.h 
    include/bbl_detail.h
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