file(GLOB HEADERS "include/koinos/*.hpp" "include/koinos/util/*.hpp")

add_library(koinos_util
   base58.cpp
   bigint.cpp
   varint.cpp
   random.cpp
   services.cpp
   hex.cpp
   ${HEADERS})

target_link_libraries(koinos_util PUBLIC Boost::headers yaml-cpp)

target_include_directories(koinos_util PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
   $<INSTALL_INTERFACE:include>
)

add_library(Koinos::util ALIAS koinos_util)

install(
   DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
   DESTINATION "include/" )

export(
   TARGETS ${KOINOS_LIB_TARGET_NAME}
   NAMESPACE Koinos::
   FILE ${KOINOS_LIB_CMAKE_PROJECT_TARGETS_FILE}
)

install(
   TARGETS ${KOINOS_LIB_TARGET_NAME}
   EXPORT ${KOINOS_LIB_TARGETS_EXPORT_NAME}
   INCLUDES DESTINATION ${KOINOS_LIB_INCLUDE_INSTALL_DIR}
   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)

install(
   EXPORT ${KOINOS_LIB_TARGETS_EXPORT_NAME}
   NAMESPACE Koinos::
   DESTINATION ${KOINOS_LIB_CONFIG_INSTALL_DIR}
)
