# Copyright 2016 Nikolas Beisemann <github@beisemann.email>
# This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
# code package.

cmake_minimum_required(VERSION 2.8)
project(walley-backend)

set(WALLEY_VERSION_NUMBER "unsupported build")
find_package(Git)
if (GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} describe
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_VARIABLE _GIT_OUTPUT
    ERROR_VARIABLE _GIT_ERROR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE
  )
  if (_GIT_ERROR STREQUAL "")
    message(STATUS "Found git revision: ${_GIT_OUTPUT}")
    set(WALLEY_VERSION_NUMBER "v${_GIT_OUTPUT}")
  endif()
endif()

add_subdirectory(src build)
add_subdirectory(docs)
