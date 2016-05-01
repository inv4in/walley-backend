# Copyright 2016 Nikolas Beisemann <github@beisemann.email>
# This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
# code package.

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
find_package(CryptoPP REQUIRED)

include_directories(${CRYPTOPP_INCLUDE_DIRS})

list(
  APPEND walley_LIBS
  ${CRYPTOPP_LIBRARIES}
)
