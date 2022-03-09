# Install script for directory: /home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler

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
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/libShiny-static.a")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib" TYPE STATIC_LIBRARY FILES "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build/lib/libShiny-static.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/Shiny.h;/usr/local/include/ShinyConfig.h;/usr/local/include/ShinyData.h;/usr/local/include/ShinyMacros.h;/usr/local/include/ShinyManager.h;/usr/local/include/ShinyNode.h;/usr/local/include/ShinyNodePool.h;/usr/local/include/ShinyNodeState.h;/usr/local/include/ShinyOutput.h;/usr/local/include/ShinyPrereqs.h;/usr/local/include/ShinyTools.h;/usr/local/include/ShinyVersion.h;/usr/local/include/ShinyZone.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/include" TYPE FILE FILES
    "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/include/Shiny.h"
    "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/include/ShinyConfig.h"
    "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/include/ShinyData.h"
    "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/include/ShinyMacros.h"
    "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/include/ShinyManager.h"
    "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/include/ShinyNode.h"
    "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/include/ShinyNodePool.h"
    "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/include/ShinyNodeState.h"
    "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/include/ShinyOutput.h"
    "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/include/ShinyPrereqs.h"
    "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/include/ShinyTools.h"
    "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/include/ShinyVersion.h"
    "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/include/ShinyZone.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build/Samples/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/orb3slam_devep1/guest2/shiny_Profiler/shinyprofiler/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
