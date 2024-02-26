add_library(util
  base64.cpp

  koinos/bigint.cpp
  koinos/varint.cpp

  koinos/util/base58.cpp
  koinos/util/base64.cpp
  koinos/util/hex.cpp
  koinos/util/random.cpp
  koinos/util/services.cpp

  base64.h

  ${PROJECT_SOURCE_DIR}/include/koinos/bigint.hpp
  ${PROJECT_SOURCE_DIR}/include/koinos/binary.hpp
  ${PROJECT_SOURCE_DIR}/include/koinos/varint.hpp

  ${PROJECT_SOURCE_DIR}/include/koinos/util/base58.hpp
  ${PROJECT_SOURCE_DIR}/include/koinos/util/base64.hpp
  ${PROJECT_SOURCE_DIR}/include/koinos/util/conversion.hpp
  ${PROJECT_SOURCE_DIR}/include/koinos/util/hex.hpp
  ${PROJECT_SOURCE_DIR}/include/koinos/util/options.hpp
  ${PROJECT_SOURCE_DIR}/include/koinos/util/overloaded.hpp
  ${PROJECT_SOURCE_DIR}/include/koinos/util/random.hpp
  ${PROJECT_SOURCE_DIR}/include/koinos/util/services.hpp
  ${PROJECT_SOURCE_DIR}/include/koinos/util/time.hpp)

target_link_libraries(
  util
    PUBLIC
      Boost::headers
    INTERFACE
      Boost::program_options
      yaml-cpp)

target_include_directories(
  util
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
    PRIVATE
      ${PROJECT_SOURCE_DIR}/src)

koinos_add_format(TARGET util)

koinos_install(TARGETS util)

install(
  DIRECTORY
    ${PROJECT_SOURCE_DIR}/include
  DESTINATION
    ${CMAKE_INSTALL_PREFIX})
