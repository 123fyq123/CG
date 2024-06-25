# Install script for directory: /usr/games/CG/Hw8/Code/CGL/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/usr/games/CG/Hw8/Code/build/CGL/src/libCGL.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CGL" TYPE FILE FILES
    "/usr/games/CG/Hw8/Code/CGL/src/CGL.h"
    "/usr/games/CG/Hw8/Code/CGL/src/vector2D.h"
    "/usr/games/CG/Hw8/Code/CGL/src/vector3D.h"
    "/usr/games/CG/Hw8/Code/CGL/src/vector4D.h"
    "/usr/games/CG/Hw8/Code/CGL/src/matrix3x3.h"
    "/usr/games/CG/Hw8/Code/CGL/src/matrix4x4.h"
    "/usr/games/CG/Hw8/Code/CGL/src/quaternion.h"
    "/usr/games/CG/Hw8/Code/CGL/src/complex.h"
    "/usr/games/CG/Hw8/Code/CGL/src/color.h"
    "/usr/games/CG/Hw8/Code/CGL/src/osdtext.h"
    "/usr/games/CG/Hw8/Code/CGL/src/viewer.h"
    "/usr/games/CG/Hw8/Code/CGL/src/base64.h"
    "/usr/games/CG/Hw8/Code/CGL/src/tinyxml2.h"
    "/usr/games/CG/Hw8/Code/CGL/src/renderer.h"
    )
endif()

