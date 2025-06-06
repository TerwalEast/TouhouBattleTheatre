# Install script for directory: C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/src/v1.6.0-fcf1526bd5.clean/DetourCrowd

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/pkgs/recastnavigation_x64-windows/debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-dbg/DetourCrowd/DetourCrowd-d.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/recastnavigation" TYPE FILE FILES
    "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/src/v1.6.0-fcf1526bd5.clean/DetourCrowd/Include/DetourCrowd.h"
    "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/src/v1.6.0-fcf1526bd5.clean/DetourCrowd/Include/DetourLocalBoundary.h"
    "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/src/v1.6.0-fcf1526bd5.clean/DetourCrowd/Include/DetourObstacleAvoidance.h"
    "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/src/v1.6.0-fcf1526bd5.clean/DetourCrowd/Include/DetourPathCorridor.h"
    "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/src/v1.6.0-fcf1526bd5.clean/DetourCrowd/Include/DetourPathQueue.h"
    "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/src/v1.6.0-fcf1526bd5.clean/DetourCrowd/Include/DetourProximityGrid.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE OPTIONAL FILES "C:/Users/xhs/source/repos/TerwalEast/TouhouBattleTheatre/vcpkg_installed/vcpkg/blds/recastnavigation/x64-windows-dbg/DetourCrowd/DetourCrowd-d.pdb")
  endif()
endif()

