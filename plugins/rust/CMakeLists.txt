add_library(bbl-rust SHARED bbl-rust.cpp)
target_link_libraries(bbl-rust babble::bbl)
target_include_directories(
  bbl-rust 
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/spdlog/include
)

install(
  TARGETS
    bbl-rust
  LIBRARY
    DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins
  RUNTIME
    DESTINATION ${CMAKE_INSTALL_PREFIX}/plugins
)