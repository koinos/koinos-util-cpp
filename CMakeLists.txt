
cmake_minimum_required(VERSION 3.10.2)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
   set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
   set(CMAKE_XCODE_ATTRIBUTE_CC           "${CMAKE_SOURCE_DIR}/ci/ccache_clang")
   set(CMAKE_XCODE_ATTRIBUTE_CXX          "${CMAKE_SOURCE_DIR}/ci/ccache_clang++")
   set(CMAKE_XCODE_ATTRIBUTE_LD           "${CMAKE_SOURCE_DIR}/ci/ccache_clang")
   set(CMAKE_XCODE_ATTRIBUTE_LDPLUSPLUS   "${CMAKE_SOURCE_DIR}/ci/ccache_clang++")
endif()

option(HUNTER_RUN_UPLOAD "Upload Hunter packages to binary cache server" OFF)

set(
   HUNTER_CACHE_SERVERS
   "https://github.com/koinos/hunter-cache"
   CACHE
   STRING
   "Default cache server"
)

include("cmake/HunterGate.cmake")

HunterGate(
   URL "https://github.com/cpp-pm/hunter/archive/v0.23.313.tar.gz"
   SHA1 "6c369e5aab2749a616917df3b23511535f413604"
   LOCAL
)

project(koinos_util VERSION 1.0.0 LANGUAGES CXX C)

#
# CONFIGURATION
#
include(GNUInstallDirs)

set(KOINOS_LIB_TARGET_NAME                ${PROJECT_NAME})
set(KOINOS_LIB_TARGET_SHORT_NAME          "util")
set(KOINOS_LIB_CONFIG_INSTALL_DIR         "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE INTERNAL "")
set(KOINOS_LIB_INCLUDE_INSTALL_DIR        "${CMAKE_INSTALL_INCLUDEDIR}")
set(KOINOS_LIB_TARGETS_EXPORT_NAME        "${PROJECT_NAME}Targets")
set(KOINOS_LIB_CMAKE_CONFIG_TEMPLATE      "cmake/config.cmake.in")
set(KOINOS_LIB_CMAKE_CONFIG_DIR           "${CMAKE_CURRENT_BINARY_DIR}")
set(KOINOS_LIB_CMAKE_VERSION_CONFIG_FILE  "${KOINOS_LIB_CMAKE_CONFIG_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(KOINOS_LIB_CMAKE_PROJECT_CONFIG_FILE  "${KOINOS_LIB_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Config.cmake")
set(KOINOS_LIB_CMAKE_PROJECT_TARGETS_FILE "${KOINOS_LIB_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Targets.cmake")
set(KOINOS_LIB_PKGCONFIG_INSTALL_DIR      "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

option(BUILD_TESTS "Build Tests" ON)

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." OFF)

# This is to force color output when using ccache with Unix Makefiles
if(${FORCE_COLORED_OUTPUT})
    if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
       add_compile_options (-fdiagnostics-color=always)
    elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" )
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(Boost_NO_BOOST_CMAKE ON)

if(COVERAGE)
  include(CodeCoverage)
  append_coverage_compiler_flags()
  setup_target_for_coverage_lcov(
     NAME coverage
     LCOV_ARGS "--quiet" "--no-external"
     EXECUTABLE koinos_util_tests
     EXCLUDE
        "build/*"
        "tests/*")
endif()

hunter_add_package(Boost COMPONENTS test program_options)
hunter_add_package(Protobuf)
hunter_add_package(yaml-cpp)

find_package(Boost CONFIG REQUIRED program_options)
find_package(Protobuf CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

add_subdirectory(libraries)
if (BUILD_TESTS)
   add_subdirectory(tests)
endif()

# Install a pkg-config file, so other tools can find this.
configure_file(
   "${CMAKE_CURRENT_SOURCE_DIR}/cmake/pkg-config.pc.in"
   "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
)

#
# INSTALL
# install header files, generate and install cmake config files for find_package()
#
include(CMakePackageConfigHelpers)

configure_file(
   "cmake/${PROJECT_NAME}ConfigVersion.cmake.in"
   ${KOINOS_LIB_CMAKE_VERSION_CONFIG_FILE}
   @ONLY
)
configure_file(
   ${KOINOS_LIB_CMAKE_CONFIG_TEMPLATE}
   ${KOINOS_LIB_CMAKE_PROJECT_CONFIG_FILE}
   @ONLY
)

install(
   FILES ${KOINOS_LIB_CMAKE_PROJECT_CONFIG_FILE} ${KOINOS_LIB_CMAKE_VERSION_CONFIG_FILE}
   DESTINATION ${KOINOS_LIB_CONFIG_INSTALL_DIR}
)

install(
   FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
   DESTINATION ${KOINOS_LIB_PKGCONFIG_INSTALL_DIR}
)
