include(CTest)

enable_testing()

koinos_add_test(koinos_util_tests
  SOURCES
    bigint_test.cpp
    main.cpp
    util_test.cpp
)

koinos_add_format_target(koinos_util_tests)

target_link_libraries(koinos_util_tests Koinos::util Boost::program_options yaml-cpp)
target_include_directories(koinos_util_tests PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
