# Copyright 2016 Nikolas Beisemann <github@beisemann.email>
# This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
# code package.

add_custom_target(docs)

find_package(Doxygen)
if (DOXYGEN_FOUND)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  )
  add_custom_target(
    docs_doxygen
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with doxygen"
  )
  add_dependencies(docs docs_doxygen)
endif()
