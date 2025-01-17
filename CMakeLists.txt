
cmake_minimum_required(VERSION 3.19.0)

cmake_policy(VERSION 3.19.0...3.27.4)

include(FetchContent)
FetchContent_Declare(
  koinos_cmake
  GIT_REPOSITORY https://github.com/koinos/koinos-cmake.git
  GIT_TAG        v1.0.6)
)
FetchContent_MakeAvailable(koinos_cmake)

include("${koinos_cmake_SOURCE_DIR}/Koinos.cmake")

project(koinos_util
  VERSION 1.0.2
  DESCRIPTION "The Koinos utility library"
  LANGUAGES CXX C)

koinos_define_version()

koinos_add_package(Boost CONFIG REQUIRED
  ADD_COMPONENTS program_options test
  FIND_COMPONENTS program_options)

koinos_add_package(yaml-cpp CONFIG REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)
