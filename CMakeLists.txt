cmake_minimum_required (VERSION 3.16)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(siren)

set(PUBLIC_HEADERS
  include/dsp.h
  include/siren.h
)

set(SRCS
  src/dsp_slow.cpp
  src/dsp_vdsp.cpp
)

IF(APPLE)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wno-deprecated-declarations -Wno-c++11-extensions")
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated -Wno-deprecated-declarations -Wno-c++11-extensions")
   FIND_LIBRARY(ACCELERATE_LIBRARY Accelerate)
   MARK_AS_ADVANCED (ACCELERATE_LIBRARY)
   SET(EXTRA_LIBS ${ACCELERATE_LIBRARY})
ENDIF (APPLE)

add_library(siren ${SRCS} ${PUBLIC_HEADERS})
TARGET_LINK_LIBRARIES( siren ${EXTRA_LIBS} )
SET_TARGET_PROPERTIES(
  siren
PROPERTIES
  VERSION 0.1.0
  SOVERSION 0.1
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
)

install(
  TARGETS siren
    LIBRARY
      DESTINATION lib
    ARCHIVE 
      DESTINATION lib
    PUBLIC_HEADER
      DESTINATION include/siren
)
