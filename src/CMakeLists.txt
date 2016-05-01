# Copyright 2016 Nikolas Beisemann <github@beisemann.email>
# This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
# code package.

include("${PROJECT_SOURCE_DIR}/cmake/use_boost.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/use_cryptopp.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/generate_clang_complete.cmake")

list(
  APPEND walley_SRC
  walley
  base64
  aes
  auxiliary
)

add_library(walley ${walley_SRC})
target_link_libraries(walley ${walley_LIBS})
