cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(keyless-migu3d VERSION 0.1.1 LANGUAGES CXX)

file(GLOB_RECURSE SOURCES src/**.cpp)
configure_file(
  include/migu/config.h.in
  "${CMAKE_CURRENT_BINARY_DIR}/include/migu/config.h"
  @ONLY
)
add_executable(keyless-migu3d ${SOURCES})
target_include_directories(keyless-migu3d
  PRIVATE
    include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)
target_compile_features(keyless-migu3d
  PRIVATE
    cxx_std_20
)

if (!MSVC)
    add_compile_options(-Wall)
endif()
