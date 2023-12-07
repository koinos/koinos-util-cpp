add_library(util
  base58.cpp
  base64.cpp
  bigint.cpp
  varint.cpp
  random.cpp
  services.cpp
  hex.cpp

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
      Boost::headers)

target_include_directories(
  util
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
)

koinos_add_format_target(util)

add_library(Koinos::util ALIAS util)

install(
  DIRECTORY
    ${PROJECT_SOURCE_DIR}/include
  DESTINATION
    include/
)
