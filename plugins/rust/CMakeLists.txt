add_library(bbl-rust SHARED bbl-rust.cpp)
target_link_libraries(bbl-rust babble::bbl)
target_include_directories(
  bbl-rust 
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/../../ext/spdlog/include
)