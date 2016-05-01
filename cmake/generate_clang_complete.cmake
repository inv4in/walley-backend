# Copyright 2016 Nikolas Beisemann <github@beisemann.email>
# This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
# code package.

get_property(include_dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
get_property(compile_defs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY COMPILE_DEFINITIONS)

set(clang_FILENAME "${CMAKE_CURRENT_SOURCE_DIR}/.clang_complete")
file(REMOVE ${clang_FILENAME})
foreach(include_dir ${include_dirs})
  file(APPEND ${clang_FILENAME} "-I${include_dir}\n")
endforeach()
file(APPEND ${clang_FILENAME} "-fPIC\n-I${CMAKE_CURRENT_BINARY_DIR}\n")
foreach(compile_def ${compile_defs})
  file(APPEND ${clang_FILENAME} "-D${compile_def}\n")
endforeach()
