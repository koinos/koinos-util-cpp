if (NOT BUILD_TESTING)
  return()
endif()

include(CTest)

koinos_add_test(
  util_tests
    SOURCES
      bigint_test.cpp
      main.cpp
      util_test.cpp)

target_link_libraries(
  util_tests
    PRIVATE
      Koinos::util)

target_include_directories(
  util_tests
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/tests>
      $<INSTALL_INTERFACE:include>)

koinos_add_format(TARGET util_tests)

koinos_coverage(
  EXECUTABLE
    util_tests
  EXCLUDE
    "tests/*")
