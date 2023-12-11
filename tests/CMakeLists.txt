if (NOT BUILD_TESTING)
  return()
endif()

include(CTest)

koinos_add_test(util_tests
  SOURCES
    bigint_test.cpp
    main.cpp
    util_test.cpp
)

target_link_libraries(util_tests Koinos::util Boost::program_options yaml-cpp)
target_include_directories(util_tests PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

koinos_add_format(
  TARGET
    util_tests
)

koinos_coverage(
  EXECUTABLE
    util_tests
  EXCLUDE
    "tests/*"
)
