# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\TheSurvivor_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\TheSurvivor_autogen.dir\\ParseCache.txt"
  "TheSurvivor_autogen"
  )
endif()
