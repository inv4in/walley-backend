# Copyright 2016 Nikolas Beisemann <github@beisemann.email>
# This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this
# code package.

find_package(Boost 1.54 COMPONENTS random filesystem system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
list(
  APPEND walley_LIBS
  ${Boost_LIBRARIES}
)
